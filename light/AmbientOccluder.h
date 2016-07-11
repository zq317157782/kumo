//
// Created by 诸谦 on 16/1/30.
//

#ifndef RAYTRACER_AMBIENTOCCLUDER_H
#define RAYTRACER_AMBIENTOCCLUDER_H

#include <OldLight.h>

#include "../oldSampler/OldSampler.h"

/**
 * 带环境遮罩的光源
 */
class AmbientOccluder:public OldLight{
private:
    RGB mIrradiance;//光 能量 入射光辐射度
    RGB mOcculuderFactor;//在光线被遮挡的情况下 光源射出的能量的缩减比率
    float mScaleFactor; //缩放因子
    Vector w,u,v;
    OldSampler* mSampler;
public:
    AmbientOccluder(const RGB &mIrradiance=RGB(0.2,0.2,0.2),const float mScaleFactor=1.0,const float occuluderFactor=0.0);
    virtual RGB L(const Intersection &sr) override ;
    void setSamper(OldSampler* sampler);//环境遮罩所使用的采样点产生器

    virtual Vector getDirection(const Intersection &sr) const;

    virtual bool inShadow(const Ray &ray, const Intersection &sr) const;
};


#endif //RAYTRACER_AMBIENTOCCLUDER_H
