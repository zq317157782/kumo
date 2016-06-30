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
		return EvaluateFresnelCond(cosi,eta,k);
}
