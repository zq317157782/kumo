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
