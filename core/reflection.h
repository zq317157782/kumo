//
// Created by Administrator on 2016/6/28 0028.
//

#ifndef RAYTRACER_REFLECTION_H
#define RAYTRACER_REFLECTION_H

#include "global.h"
#include "geometry.h"
#include "RGB.h"
#include "diffgeom.h"
//反射坐标系 三个标准正交基是两切线和法线

//cos(t)=(N.DOT.w)==(0,0,1).dot.w=w.z
inline float CosTheta(const Vector &w) {
	return w.z;
}
inline float AbsCosTheta(const Vector &w) {
	return fabsf(w.z);
}
//sin(t)2+cos(t)2==1
inline float SinTheta2(const Vector& w) {
	float cosw = CosTheta(w);
	return max(0.0f, 1.0f - cosw * cosw);
}
inline float SinTheta(const Vector& w) {
	return sqrtf(SinTheta2(w));
}
inline float CosPhi(const Vector& w) {
	float sint = SinTheta(w);
	if (sint == 0.f)
		return 1.f;
	return Clamp(w.x / sint, -1.f, 1.f);
}

inline float SinPhi(const Vector& w) {
	float sint = SinTheta(w);
	if (sint == 0.f)
		return 0.f;
	return Clamp(w.y / sint, -1.f, 1.f);
}

enum BxDFType {
	BSDF_REFLECTION = 1 << 0,  //反射
	BSDF_TRANSMISSION = 1 << 1,  //折射
	BSDF_DIFFUSE = 1 << 2,  //漫反射
	BSDF_GLOSSY = 1 << 3,  //混合
	BSDF_SPECULAR = 1 << 4,  //镜面反射
	BSDF_ALL_TYPES = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR,
	BSDF_ALL_REFLECTION = BSDF_REFLECTION | BSDF_ALL_TYPES,
	BSDF_ALL_TRANSMISSION = BSDF_TRANSMISSION | BSDF_ALL_TYPES,
	BSDF_ALL = BSDF_ALL_REFLECTION | BSDF_ALL_TRANSMISSION
};

//BxDF   BRDF 和BTDF的基类
class BxDF {
public:
	const BxDFType type;
	BxDF(BxDFType t) :
			type(t) {
	}

	//判断标记是否符合
	bool MatchesFlag(BxDFType flags) {
		return (type & flags) == type;
	}

	virtual RGB f(const Vector &wo, const Vector &wi) const=0;  //给非狄克尔分布的版本
	virtual RGB sample_f(const Vector& wo, Vector* wi, float u1, float u2,
			float *pdf) const {
		return RGB(0);
	}  //给狄克尔分布和蒙特卡洛积分使用的版本

	virtual RGB rho(const Vector& wo, int nSamples, const float*samples) const {
		return RGB(0);
	}
	;  //hemispherical-directional reflectance
	virtual RGB rho(int nSamples, const float *samples1,
			const float *samples2) const {
		return RGB(0);
	}
	;  //hemispherical-hemispherical reflectance

	virtual float pdf(const Vector& wo, const Vector& wi) const {
		return 0;
	}
	;
	//通过入射光线和出射光线来计算概率分布

	virtual ~BxDF(){}
};

//BRDF->BTDF Adapter
class BRDFToBTDF: public BxDF {
private:
	BxDF* mBrdf;
public:
	BRDFToBTDF(BxDF* brdf) :
			BxDF(
					(BxDFType) (brdf->type
							^ (BSDF_REFLECTION | BSDF_TRANSMISSION))), mBrdf(
					brdf) {
	}
	RGB f(const Vector &wo, const Vector &wi) const override; //给非狄克尔分布的版本
	RGB sample_f(const Vector& wo, Vector* wi, float u1, float u2,
			float *pdf) const override; //给狄克尔分布和蒙特卡洛积分使用的版本
	RGB rho(const Vector &w, int nSamples, const float *samples) const
			override {  //hemispherical-directional reflectance
		return mBrdf->rho(otherHemisphere(w), nSamples, samples);
	}
	RGB rho(int nSamples, const float *samples1, const float *samples2) const
			override {  //hemispherical-hemispherical reflectance
		return mBrdf->rho(nSamples, samples1, samples2);
	}
	static Vector otherHemisphere(const Vector& w) {
		return Vector(w.x, w.y, -w.z);
	}

};

class ScaledBxDF: public BxDF {
private:
	BxDF* mBxdf;
	RGB mScale;
public:
	ScaledBxDF(BxDF* bxdf, const RGB& s) :
			BxDF(bxdf->type), mBxdf(bxdf), mScale(s) {
	}
	RGB f(const Vector &wo, const Vector &wi) const override;  //给非狄克尔分布的版本
	RGB sample_f(const Vector& wo, Vector* wi, float u1, float u2,
			float *pdf) const override;  //给狄克尔分布和蒙特卡洛积分使用的版本
	RGB rho(const Vector &w, int nSamples, const float *samples) const
			override {  //hemispherical-directional reflectance
		return mScale * mBxdf->rho(w, nSamples, samples);
	}
	RGB rho(int nSamples, const float *samples1, const float *samples2) const
			override {  //hemispherical-hemispherical reflectance
		return mScale * mBxdf->rho(nSamples, samples1, samples2);
	}
};

// 绝缘体Fresnel反射系数
RGB EvaluateFresnelDiel(float cosi, float cost, const RGB &etai,
		const RGB &etat);
//导电体Fresnel反射系数
RGB EvaluateFresnelCond(float cosi, const RGB &n, const RGB &k);

//代表Fresnel系数的类型
class Fresnel {
public:
	virtual RGB Evaluate(float cosi) const = 0;  //计算Fresnel反射系数
	virtual ~Fresnel() {
	}
	;
};

//代表导电体Fresnel系数
class FresnelConductor: public Fresnel {
private:
	RGB mEta; //入射材质折射系数
	RGB mK;   //出射材质吸收系数
public:
	FresnelConductor(const RGB& e, const RGB& kk) :
			mEta(e), mK(kk) {
	}
	RGB Evaluate(float cosi) const override;
};

//todo 理解为什么PBRT这里eta_i和eta_t要用float ,而不是像FresnelConductor一样使用光谱来表示
class FresnelDielectric: public Fresnel {
private:
	float mEtaI;   //入射材质的折射系数
	float mEtaT;   //出射材质的折射系数
public:
	FresnelDielectric(float i, float t) :
			mEtaI(i), mEtaT(t) {
	}
	RGB Evaluate(float cosi) const override;

};

//完美镜面反射BRDF
class SpecularReflection: public BxDF {
private:
	RGB mScale;  //缩放系数
	const Fresnel*mFresnel;
public:
	SpecularReflection(const RGB& r, Fresnel* f) :
			BxDF(BxDFType(BSDF_REFLECTION | BSDF_SPECULAR)), mScale(r), mFresnel(
					f) {
	}

	virtual RGB f(const Vector &wo, const Vector &wi) const override { //给非狄克尔分布的版本
		return 0.f; //因为是完美镜面反射，所以直接返回0;
	}
	;
	RGB sample_f(const Vector& wo, Vector* wi, float u1, float u2,
			float *pdf) const override; //这个是镜面反射需要实现的函数
};

//镜面折射BTDF
class SpecularTransmission: public BxDF {
private:
	RGB mScale;
	FresnelDielectric mFresnel; //因为是折射 所以只有绝缘体会发生
	float mEtaI, mEtaT;
public:
	SpecularTransmission(const RGB& t, float ei, float et) :
			BxDF(BxDFType(BSDF_TRANSMISSION | BSDF_SPECULAR)), mScale(t), mFresnel(
					ei, et) {
		mEtaI = ei; //入射折射系数
		mEtaT = et; //出射折射系数
	}

	virtual RGB f(const Vector &wo, const Vector &wi) const override { //给非狄克尔分布的版本
		return 0.f; //因为是完美镜面折射，所以直接返回0;
	}
	;

	RGB sample_f(const Vector& wo, Vector* wi, float u1, float u2,
			float *pdf) const override; //这个是镜面折射需要实现的函数
};

//漫反射brdf
class Lambertian: public BxDF {
private:
	RGB mR;
public:
	Lambertian(const RGB& r) :
			BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE)), mR(r) {
	}
	RGB f(const Vector &wo, const Vector &wi) const override { //给非狄克尔分布的版本
		return mR / M_INV_PI;
	}

	RGB rho(const Vector &w, int nSamples, const float *samples) const
			override {  //hemispherical-directional reflectance
		return mR;
	}
	RGB rho(int nSamples, const float *samples1, const float *samples2) const
			override {  //hemispherical-hemispherical reflectance
		return mR;
	}
};

//OrenNayar模型
class OrenNayar: public BxDF {
private:
	RGB mR;
	float A, B;
public:
	OrenNayar(const RGB &reflectance, float sig) :
			BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE)), mR(reflectance) {
		//计算公式中的A和B部分
		float sigma = Radians(sig);
		float sigma2 = sigma * sigma;
		A = 1.f - (sigma2 / (2.0f * (sigma2 + 0.33f)));
		B = 0.45f * sigma2 / (sigma2 + 0.09f);
	}


	//代码基本上是从pbrt那边照搬过来
	RGB f(const Vector &wo, const Vector &wi) const override { //给非狄克尔分布的版本
		//法线坐标系下的操作
		float sinthetai = SinTheta(wi);
		float sinthetao = SinTheta(wo);

		//计算max(cos(t-t))
		float maxcos = 0.f;
		if (sinthetai > 1e-4 && sinthetao > 1e-4) {
			float sinphii = SinPhi(wi), cosphii = CosPhi(wi);
			float sinphio = SinPhi(wo), cosphio = CosPhi(wo);
			float dcos = cosphii * cosphio + sinphii * sinphio;
			maxcos = max(0.f, dcos);
		}
		//计算sin 和 tan项
		float sinalpha, tanbeta;
		if (AbsCosTheta(wi) > AbsCosTheta(wo)) {
			sinalpha = sinthetao;
			tanbeta = sinthetai / AbsCosTheta(wi);
		} else {
			sinalpha = sinthetai;
			tanbeta = sinthetao / AbsCosTheta(wo);
		}
		return mR * M_INV_PI * (A + B * maxcos * sinalpha * tanbeta);
	}
};


//微平面分布
class MicrofacetDistribution{
public:
	virtual ~MicrofacetDistribution(){}
	virtual float D(const Vector &wh) const=0;//传入半角向量 返回与该半角向量垂直的位平面的分布概率
};

//基于Torrance-Sparrow Modle的微平面结构
class Microfacet:public BxDF{
private:
	RGB mR;//调节反射总比例
	Fresnel * mFresnel;//菲涅尔系数
	MicrofacetDistribution * mDistribution;//微平面的法线分布函数
public:
	Microfacet(const RGB& reflectance,Fresnel* fresnel,MicrofacetDistribution* distribution);
	float G(const Vector &wo, const Vector &wi, const Vector &wh) const;//几何衰减实数  公式:PBRT P455
	RGB f(const Vector &wo, const Vector &wi) const override ;
};


//第一个微平面分布 公式在p455
class Blinn:public MicrofacetDistribution{
private:
	float mE;//指数
public:
	Blinn(float e):mE(e){};
	virtual float D(const Vector &wh) const override{
		float cosh=CosTheta(wh);
		return (mE+2.0f)*M_INV_TWO_PI*powf(cosh,mE);
	}
};

//todo 优先编写BSDF

//BSDF 双向散射分布函数
//The BSDF class represents a collection of BRDFs and BTDFs
class BSDF{
private:
	Normal mNN,mNG;//着色法线，几何法线
	Vector mSN,mTN;//次切向量，切向量
	int mNumBxdf;//BxDF的个数
#define MAX_BxDFS 8
    BxDF *mBxdfs[MAX_BxDFS];
public:
	const float eta;//材质的折射系数
	const DifferentialGeometry dgShading;//着色微分几何
	BSDF(const DifferentialGeometry& dg,const Normal& ng,float e);

	void Add(BxDF *bxdf);//加入BxDF
	Vector WorldToLocal(const Vector& w) const;
	Vector LocalToWorld(const Vector& w) const;

	int NumComponents() const;
	int NumComponents(BxDFType flags) const;

	RGB f(const Vector &woWorld, const Vector &wiWorld, BxDFType flags = BSDF_ALL) const;
};

#endif //RAYTRACER_REFLECTION_H
