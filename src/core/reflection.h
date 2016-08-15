//
// Created by Administrator on 2016/6/28 0028.
//

#ifndef RAYTRACER_REFLECTION_H
#define RAYTRACER_REFLECTION_H

#include "global.h"
#include "geometry.h"
#include "RGB.h"
#include "diffgeom.h"
#include "montecarlo.h"
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

//判断两个向量是否在同一半球
inline bool SameHemisphere(const Vector &w, const Vector &wp) {
	return w.z * wp.z > 0.f;
}

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
	//给狄克尔分布和蒙特卡洛积分使用的版本
	virtual RGB Sample_f(const Vector& wo, Vector* wi, float u1, float u2,
			float *pdf) const {
		*wi = CosSampleHemisphere(u1, u2);
		if (wo.z < 0.0f)
			wi->z *= -1.0f;
		*pdf = Pdf(wo, *wi);
		return f(wo, *wi);
	}

	//通过入射光线和出射光线来计算概率分布
	virtual float Pdf(const Vector& wo, const Vector& wi) const {
		return SameHemisphere(wo, wi) ? AbsCosTheta(wi) * M_INV_PI : 0.0f;
	}

	virtual RGB rho(const Vector& wo, int nSamples, const float*samples) const {
		Vector wi;
		float pdf;
		RGB sum(0);
		//采用蒙特卡诺方式来计算积分
		for (int i = 0; i < nSamples; ++i) {
			RGB f = Sample_f(wo, &wi, samples[i * 2], samples[i * 2 + 1], &pdf);
			if (pdf > 0)
				sum += f * AbsCosTheta(wi) / pdf;
		}
		return sum / nSamples;
	}
	;  //hemispherical-directional reflectance
	virtual RGB rho(int nSamples, const float *samples1,
			const float *samples2) const {
		RGB sum(0);
		for (int i = 0; i < nSamples; ++i) {
			float pdf_o = M_INV_TWO_PI;
			float pdf_i = 0;
			Vector wo = UniformSampleHemisphere(samples1[i * 2],
					samples1[i * 2 + 1]);
			Vector wi;
			RGB f = Sample_f(wo, &wi, samples2[i * 2], samples2[i * 2 + 1],
					&pdf_i);
			sum += f * AbsCosTheta(wi) * AbsCosTheta(wo) / (pdf_i * pdf_o);
		}
		return sum / (M_PI * nSamples);
	}
	;  //hemispherical-hemispherical reflectance

	virtual ~BxDF() {
	}
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
	RGB Sample_f(const Vector& wo, Vector* wi, float u1, float u2,
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
	RGB Sample_f(const Vector& wo, Vector* wi, float u1, float u2,
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

class FresnelNoOp : public Fresnel {
public:
    RGB Evaluate(float) const { return RGB(1); }
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
		return 0.0f; //因为是完美镜面反射，所以直接返回0;
	}

	RGB Sample_f(const Vector& wo, Vector* wi, float u1, float u2,
			float *pdf) const override; //这个是镜面反射需要实现的函数

	virtual float Pdf(const Vector& wo, const Vector& wi) const override {
		return 0.0f;
	}
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

	RGB Sample_f(const Vector& wo, Vector* wi, float u1, float u2,
			float *pdf) const override; //这个是镜面折射需要实现的函数
	virtual float Pdf(const Vector& wo, const Vector& wi) const override {
		return 0.0f;
	}
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
class MicrofacetDistribution {
public:
	virtual ~MicrofacetDistribution() {
	}
	virtual float D(const Vector &wh) const=0;		//传入半角向量 返回与该半角向量垂直的位平面的分布概率

	//用来采样微平面法线分布的函数
	virtual void Sample_f(const Vector &wo, Vector *wi, float u1, float u2,
			float *pdf) const = 0;
	//返回概率密度
	virtual float Pdf(const Vector &wo, const Vector &wi) const = 0;
};

//基于Torrance-Sparrow Modle的微平面结构
class Microfacet: public BxDF {
private:
	RGB mR;		//调节反射总比例
	Fresnel * mFresnel;		//菲涅尔系数
	MicrofacetDistribution * mDistribution;		//微平面的法线分布函数
public:
	Microfacet(const RGB& reflectance, Fresnel* fresnel,
			MicrofacetDistribution* distribution);
	float G(const Vector &wo, const Vector &wi, const Vector &wh) const;//几何衰减实数  公式:PBRT P455
	RGB f(const Vector &wo, const Vector &wi) const override;

	//微平面使用的PDF是法线分布
	virtual RGB Sample_f(const Vector& wo, Vector* wi, float u1, float u2,
			float *pdf) const override {
		mDistribution->Sample_f(wo, wi, u1, u2, pdf);		//采样法线分布
		if (!SameHemisphere(wo, *wi))
			return RGB(0.0f); //因为采样的时候是按照半角来采样的，最后可能得到入射和出射不在同一半角的情况，则返回0  因为微平面并没有考虑折射的情况
		return f(wo, *wi);
	}

	//通过入射光线和出射光线来计算概率分布
	virtual float Pdf(const Vector& wo, const Vector& wi) const override {
		if (!SameHemisphere(wo, wi))
			return 0.f;
		return mDistribution->Pdf(wo, wi);
	}
};

//第一个微平面分布 公式在p455
class Blinn: public MicrofacetDistribution {
private:
	float mE;		//指数
public:
	Blinn(float e) :
			mE(e) {
	}
	;
	virtual float D(const Vector &wh) const override {
		float cosh = CosTheta(wh);
		return (mE + 2.0f) * M_INV_TWO_PI * powf(cosh, mE);
	}

	virtual void Sample_f(const Vector &wo, Vector *wi, float u1, float u2,
			float *pdf) const override {
		float cosTheta = powf(u1, 1.f / (mE + 1));		//cos θh == ξ1开根n+1
		float sinTheta = sqrtf(max(0.0f, (1.0f - cosTheta * cosTheta)));
		float phi = u2 * 2.f * M_PI;
		Vector wh = SphericalDirection(sinTheta, cosTheta, phi);		//获得半角向量
		if (!SameHemisphere(wo, wh))
			wh = -wh;		//使半角向量和出射光线在同一半球中
		*wi = -wo + 2.f * Dot(wo, wh) * wh;		//利用反射公式，计算入射光线
		//计算Blinn的概率密度 这里是把wh的概率密度转换到wi的分布后再计算的(所以要用到分布之间的转换)
		float blinn_pdf = ((mE + 1.f) * powf(cosTheta, mE))
				/ (2.f * M_PI * 4.f * Dot(wo, wh));
		if (Dot(wo, wh) <= 0.f)
			blinn_pdf = 0.f;		//这一步无法理解
		*pdf = blinn_pdf;
	}

	virtual float Pdf(const Vector &wo, const Vector &wi) const override {
		Vector wh = Normalize(wo + wi);
		float costheta = AbsCosTheta(wh);
		float blinn_pdf = ((mE + 1.f) * powf(costheta, mE)) //p(wh)=(n+1)*cos(t)^n  p(wi)=dwh/dwip(wh)
		/ (2.f * M_PI * 4.f * Dot(wo, wh));
		if (Dot(wo, wh) <= 0.f)
			blinn_pdf = 0.f;
		return blinn_pdf;
	}
};

//各项异性法线分布函数 实现直接取自PBRT,以后需要仔细研究
class Anisotropic : public MicrofacetDistribution {
public:
    Anisotropic(float x, float y) {
        ex = x; ey = y;
        if (ex > 10000.f || isnan(ex)) ex = 10000.f;
        if (ey > 10000.f || isnan(ey)) ey = 10000.f;
    }
    float D(const Vector &wh) const {
        float costhetah = AbsCosTheta(wh);
        float d = 1.f - costhetah * costhetah;
        if (d == 0.f) return 0.f;
        float e = (ex * wh.x * wh.x + ey * wh.y * wh.y) / d;
        return sqrtf((ex+2.f) * (ey+2.f)) * M_INV_TWO_PI * powf(costhetah, e);
    }
    void Sample_f(const Vector &wo, Vector *wi, float u1, float u2, float *pdf) const;
    float Pdf(const Vector &wo, const Vector &wi) const;
    void sampleFirstQuadrant(float u1, float u2, float *phi, float *costheta) const;
private:
    float ex, ey;
};


//代表一个BSDF的采样点
struct BSDFSample {
	float uDir[2]; //两个随机样本
	float uComponent; //代表哪个BxDF被选中用来计算方向的样本

	BSDFSample(){}

	BSDFSample(Random & rand) {
		uDir[0] = rand.RandomFloat();
		uDir[1] = rand.RandomFloat();
		uComponent = rand.RandomFloat();
	}

	BSDFSample(float u1, float u2, float uC) {
		uDir[0] = u1;
		uDir[1] = u2;
		uComponent = uC;
	}

	//对应Sample的版本
	BSDFSample(const Sample *sample, const BSDFSampleOffsets &offsets,
				uint32_t num);
};

//为了使得BSDFSample和Sample结合在一起的结构
struct BSDFSampleOffsets {
	BSDFSampleOffsets() {
	}
	BSDFSampleOffsets(int count, Sample *sample);
	int nSamples, componentOffset, dirOffset;
};

//BSDF 双向散射分布函数
//The BSDF class represents a collection of BRDFs and BTDFs
class BSDF {
private:
	Normal mNN, mNG;		//着色法线，几何法线
	Vector mSN, mTN;		//次切向量，切向量
	int mNumBxdf;		//BxDF的个数
#define MAX_BxDFS 8
	BxDF *mBxdfs[MAX_BxDFS];
public:
	const float eta;		//材质的折射系数
	const DifferentialGeometry dgShading;		//着色微分几何
	BSDF(const DifferentialGeometry& dg, const Normal& ng, float e = 1.f);//e是材质的折射率

	void Add(BxDF *bxdf);		//加入BxDF
	Vector WorldToLocal(const Vector& w) const;
	Vector LocalToWorld(const Vector& w) const;

	int NumComponents() const;
	int NumComponents(BxDFType flags) const;

	RGB f(const Vector &woWorld, const Vector &wiWorld, BxDFType flags =
			BSDF_ALL) const;
	//采样BSDF的函数
	RGB Sample_f(const Vector &wo, Vector *wi, const BSDFSample &bsdfSample,
	                      float *pdf, BxDFType flags = BSDF_ALL,
	                      BxDFType *sampledType = nullptr) const;

	float Pdf(const Vector &woW, const Vector &wiW,
	        BxDFType flags= BSDF_ALL) const;

	RGB rho(const Vector &wo, Random &rng, BxDFType flags = BSDF_ALL,
	                 int sqrtSamples = 6) const;
};



//为BSDF分配空间的宏定义
#define BSDF_ALLOC(arena, Type) new (arena.Alloc(sizeof(Type))) Type

#endif //RAYTRACER_REFLECTION_H
