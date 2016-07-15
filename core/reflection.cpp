//
// Created by Administrator on 2016/6/28 0028.
//

#include "reflection.h"
#include "RGB.h"


RGB BRDFToBTDF::f(const Vector &wo,const Vector &wi) const{
    return mBrdf->f(wo,otherHemisphere(wi));
}

RGB BRDFToBTDF::sample_f(const Vector& wo,Vector* wi,float u1,float u2,float *pdf) const {
    RGB ret=mBrdf->sample_f(wo,wi,u1,u2,pdf);
    *wi=otherHemisphere(*wi);//改变采样后的入射光的方向
    return ret;
}

RGB ScaledBxDF::f(const Vector &wo,const Vector &wi) const{
    return mScale*mBxdf->f(wo,wi);
}

RGB ScaledBxDF::sample_f(const Vector& wo,Vector* wi,float u1,float u2,float *pdf) const {
   return mScale*mBxdf->sample_f(wo,wi,u1,u2,pdf);
}


RGB EvaluateFresnelDiel(float cosi, float cost, const RGB &etai,
                const RGB &etat) {
	//以下是计算绝缘体Fresnel系数的公式实现
	RGB Rparl = ((etat * cosi) - (etai * cost)) /
                     ((etat * cosi) + (etai * cost));
	RGB Rperp = ((etai * cosi) - (etat * cost)) /
                     ((etai * cosi) + (etat * cost));
    return (Rparl*Rparl + Rperp*Rperp) / 2.f;
}


RGB EvaluateFresnelCond(float cosi, const RGB &eta, const RGB &k) {
	//导电体的Fresnel反射系数计算
	RGB tmp = (eta*eta + k*k) * cosi*cosi;
	RGB Rparl2 = (tmp - (2.f * eta * cosi) + 1) /
                      (tmp + (2.f * eta * cosi) + 1);
	RGB tmp_f = eta*eta + k*k;
	RGB Rperp2 =
        (tmp_f - (2.f * eta * cosi) + cosi*cosi) /
        (tmp_f + (2.f * eta * cosi) + cosi*cosi);
    return (Rparl2 + Rperp2) / 2.f;
}


RGB FresnelConductor::Evaluate(float cosi) const{
	//计算入射角度与法线之间的角度的余弦是cosi的导电体Fresnel反射系数
	return EvaluateFresnelCond(cosi,mEta,mK);
}

RGB FresnelDielectric::Evaluate(float cosi) const{
	cosi=Clamp(cosi,-1.f,1.f);//保护cosi的值
	bool entering=cosi>0.;//用来判断射线是入射的还是出射的
	float ei=mEtaI,et=mEtaT;
	if(!entering)
		swap(ei,et);//如果是出射的话，交换折射系数的顺序
	//Snell's law
	float sint=ei/et*sqrtf(max(0.f,1.f-cosi*cosi));
	if(sint>=1){
		return 1;//完全反射，没有折射
	}
	else{
		float cost=sqrtf(max(0.f,1.f-sint*sint));
		return EvaluateFresnelDiel(fabsf(cosi),cost,ei,et);//返回反射系数
	}
}

RGB SpecularReflection::sample_f(const Vector& wo,Vector* wi,float u1,float u2,float *pdf) const{
	*wi=Vector(-wo.x,-wo.y,wo.z);//反射向量
	*pdf=1.f;//概率分布为1
	return mFresnel->Evaluate(CosTheta(wo))*mScale/AbsCosTheta(*wi); //镜面反射的brdf公式
}


RGB SpecularTransmission::sample_f(const Vector& wo,Vector* wi,float u1,float u2,float *pdf) const{
	  bool entering = CosTheta(wo) > 0.;
	  float ei = mEtaI, et = mEtaT;
	  if (!entering)//判断wo是从外面射入还是从内部射出
	     swap(ei, et);
	  //根据Snell's law 计算折射方向
	  float sini2 = SinTheta2(wo);
	  float eta = ei / et;
	  float sint2 = eta * eta * sini2;

	  if (sint2 >= 1.) return 0.;//所有的光线全部反射，所以没有折射
	  float cost = sqrtf(max(0.f, 1.f - sint2));
	  if (entering) cost = -cost; //设置符号
	  float sintOverSini = eta;
	  *wi = Vector(sintOverSini * -wo.x, sintOverSini * -wo.y, cost);
	  *pdf = 1.f;
	  RGB F = mFresnel.Evaluate(CosTheta(wo));//计算反射系数
	  return (ei*ei)/(et*et) * (RGB(1.0f)-F) *mScale/AbsCosTheta(*wi);
}


Microfacet::Microfacet(const RGB& reflectance,Fresnel* fresnel,MicrofacetDistribution* distribution):BxDF(BxDFType(BSDF_REFLECTION | BSDF_GLOSSY)),mR(reflectance),mFresnel(fresnel),mDistribution(distribution){

}

float Microfacet::G(const Vector &wo, const Vector &wi, const Vector &wh) const{
	float NdotWh=AbsCosTheta(wh);//半角和法线之间的cos值  相当于是点乘
	float NdotWo=AbsCosTheta(wo);//出射方向与法线
	float NdotWi=AbsCosTheta(wi);//入射方向与法线
	float WodotWh=AbsDot(wh,wo);//出射和半角向量之间的点乘
	return min(1.0f,min((2.f * NdotWh * NdotWo / WodotWh),
            (2.f * NdotWh * NdotWi / WodotWh)));
}


//这里使用的就是Torrance-Sparrow Modle的公式
RGB Microfacet::f(const Vector &wo, const Vector &wi) const{
	float cosO=AbsCosTheta(wo);
	if(cosO==0) return RGB(0);
	float cosI=AbsCosTheta(wi);
	if(cosI==0) return RGB(0);
	cout<<"wi: "<<wi.x<<" "<<wi.y<<" "<<wi.z<<" "<<endl;
	cout<<"wo: "<<wo.x<<" "<<wo.y<<" "<<wo.z<<" "<<endl;
	Vector wh=wi+wo;
	if(wh.x==0&&wh.y==0&&wh.z==0) return RGB(0);
	wh=Normalize(wh);
	float cosH=Dot(wi,wh);
	RGB F= mFresnel->Evaluate(cosH);
//	cout<<"fresnel: "<<F.r<<" "<<F.g<<" "<<F.b<<" "<<endl;
//	cout<<"mDistribution: "<<mDistribution->D(wh)<<endl;
//	cout<<"G(wo,wi,wh): "<<G(wo,wi,wh)<<endl;
	return mR*F*mDistribution->D(wh)*G(wo,wi,wh)/(4.0f*cosO*cosI);
}
