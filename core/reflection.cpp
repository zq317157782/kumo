//
// Created by Administrator on 2016/6/28 0028.
//

#include "reflection.h"

RGB BRDFToBTDF::f(const Vector &wo,const Vector &wi) const{
    return mBrdf->f(wo,otherHemisphere(wi));
}

RGB BRDFToBTDF::sample_f(const Vector& wo,Vector* wi,float u1,float u2,float *pdf) const {
    RGB ret=mBrdf->sample_f(wo,wi,u1,u2,pdf);
    *wi=otherHemisphere(*wi);//改变采样后的入射光的方向
    return ret;
}