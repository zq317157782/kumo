//
// Created by 诸谦 on 15/12/27.
//

#include "PointLight.h"
#include "../scene/Scene.h"
#include "../common/Ray.h"

PointLight::PointLight(const Vector3 &mPosition, const RGB &mIrradiance, float mScaleFactor) : mPosition(mPosition),
                                                                                   mIrradiance(mIrradiance),
                                                                                   mScaleFactor(mScaleFactor) {

}

Vector3 PointLight::getDirection(const ShadeRec &sr) const {
    return (mPosition-sr.hitPoint).normalize();
}

RGB PointLight::L(const ShadeRec &sr) const {
    return mScaleFactor*mIrradiance; //这里没有带距离衰减系数
}

bool PointLight::inShadow(const Ray &ray, const ShadeRec &sr) const {
    unsigned long num_obj=sr.scene.getPrimitiveNum();
    double t=9999999;
    double d =mPosition.distance2(ray.position);
    for(int i=0;i<num_obj;++i){
        if(sr.scene.getPrimitive(i)->castShadow()&&sr.scene.getPrimitive(i)->shadowHit(ray,t) && t<d){
            return true;
        }
    }
    return false;

}
