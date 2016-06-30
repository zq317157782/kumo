//
// Created by Administrator on 2016/6/28 0028.
//

#ifndef RAYTRACER_REFLECTION_H
#define RAYTRACER_REFLECTION_H

#include "global.h"
#include "geometry.h"
#include "RGB.h"
//反射坐标系 三个标准正交基是两切线和法线

//cos(t)=(N.DOT.w)==(0,0,1).dot.w=w.z
inline float CosTheta(const Vector &w){
    return w.z;
}
inline float AbsCosTheta(const Vector &w) {
    return fabsf(w.z);
}
//sin(t)2+cos(t)2==1
inline float SinTheta2(const Vector& w){
    float cosw=CosTheta(w);
    return max(0.0f,1.0f-cosw*cosw);
}
inline float SinTheta(const Vector& w){
    return sqrtf(SinTheta2(w));
}
inline float CosPhi(const Vector& w){
    float sint=SinTheta(w);
    if(sint==0.f) return 1.f;
    return Clamp(w.x/sint,-1.f,1.f);
}

inline float SinPhi(const Vector& w){
    float sint=SinTheta(w);
    if(sint==0.f) return 0.f;
    return Clamp(w.y/sint,-1.f,1.f);
}

enum BxDFType {
    BSDF_REFLECTION   = 1<<0,  //反射
    BSDF_TRANSMISSION = 1<<1,  //折射
    BSDF_DIFFUSE      = 1<<2,  //漫反射
    BSDF_GLOSSY       = 1<<3,  //混合
    BSDF_SPECULAR     = 1<<4,  //镜面反射
    BSDF_ALL_TYPES        = BSDF_DIFFUSE |
                            BSDF_GLOSSY |
                            BSDF_SPECULAR,
    BSDF_ALL_REFLECTION   = BSDF_REFLECTION |
                            BSDF_ALL_TYPES,
    BSDF_ALL_TRANSMISSION = BSDF_TRANSMISSION |
                            BSDF_ALL_TYPES,
    BSDF_ALL              = BSDF_ALL_REFLECTION |
                            BSDF_ALL_TRANSMISSION
};

//BxDF   BRDF 和BTDF的基类
class BxDF{
public:
    const BxDFType type;
    BxDF(BxDFType t):type(t){}

    //判断标记是否符合
    bool MatchesFlag(BxDFType flags){
        return (type&flags)==type;
    }

    virtual RGB f(const Vector &wo,const Vector &wi) const=0;//给非狄克尔分布的版本
    virtual RGB sample_f(const Vector& wo,Vector* wi,float u1,float u2,float *pdf) const;//给狄克尔分布和蒙特卡洛积分使用的版本

    virtual RGB rho(const Vector& wo,int nSamples,const float*samples) const;//hemispherical-directional reflectance
    virtual RGB rho(int nSamples, const float *samples1,const float *samples2) const;//hemispherical-hemispherical reflectance

    virtual float pdf(const Vector& wo,const Vector& wi) const=0; //通过入射光线和出射光线来计算概率分布
};

//BRDF->BTDF Adapter
class BRDFToBTDF:public BxDF{
private:
    BxDF* mBrdf;
public:
    BRDFToBTDF(BxDF* brdf):BxDF((BxDFType)(brdf->type^(BSDF_REFLECTION|BSDF_TRANSMISSION))),mBrdf(brdf){}
    RGB f(const Vector &wo,const Vector &wi) const override=0;//给非狄克尔分布的版本
    RGB sample_f(const Vector& wo,Vector* wi,float u1,float u2,float *pdf) const override;//给狄克尔分布和蒙特卡洛积分使用的版本
    RGB rho(const Vector &w, int nSamples, const float *samples) const override{  //hemispherical-directional reflectance
        return mBrdf->rho(otherHemisphere(w), nSamples, samples);
    }
    RGB rho(int nSamples, const float *samples1, const float *samples2) const override{  //hemispherical-hemispherical reflectance
        return mBrdf->rho(nSamples, samples1, samples2);
    }
    static Vector otherHemisphere(const Vector& w){return Vector(w.x,w.y,-w.z);}

};


class ScaledBxDF:public BxDF{
private:
    BxDF* mBxdf;
    RGB mScale;
public:
    ScaledBxDF(BxDF* bxdf, const RGB& s):BxDF(bxdf->type),mBxdf(bxdf),mScale(s){}
    RGB f(const Vector &wo,const Vector &wi) const override=0;//给非狄克尔分布的版本
    RGB sample_f(const Vector& wo,Vector* wi,float u1,float u2,float *pdf) const override;//给狄克尔分布和蒙特卡洛积分使用的版本
    RGB rho(const Vector &w, int nSamples, const float *samples) const override{  //hemispherical-directional reflectance
        return mScale*mBxdf->rho(w, nSamples, samples);
    }
    RGB rho(int nSamples, const float *samples1, const float *samples2) const override{  //hemispherical-hemispherical reflectance
        return mScale*mBxdf->rho(nSamples, samples1, samples2);
    }
};

#endif //RAYTRACER_REFLECTION_H
