//
// Created by 诸谦 on 15/12/27.
//

#include "PointLight.h"
#include "../scene/Scene.h"

PointLight::PointLight(const Point &mPosition, const RGB &mIrradiance,const float mScaleFactor) : mPosition(mPosition),
                                                                                   mIrradiance(mIrradiance),
                                                                                   mScaleFactor(mScaleFactor) {

}

Vector PointLight::getDirection(const ShadeRec &sr) const {
    return Normalize(mPosition-sr.hitPoint);
}

RGB PointLight::L(const ShadeRec &sr){
    return mScaleFactor*mIrradiance; //这里没有带距离衰减系数
}

bool PointLight::inShadow(const Ray &ray, const ShadeRec &sr) const {
    unsigned long num_obj=sr.scene.getPrimitiveNum();
    double t=9999999;
    double d = (mPosition-ray.o).Length();
    for(int i=0;i<num_obj;++i){
        if(sr.scene.getPrimitive(i)->castShadow()&&sr.scene.getPrimitive(i)->shadowHit(ray,t) && t<d){
            return true;
        }
    }
    return false;

}
