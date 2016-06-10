//
// Created by 诸谦 on 15/12/27.
//

#include "Directional.h"
#include "../scene/Scene.h"

Directional::Directional(const RGB &mIrradiance, const Vector &mDirection,const float mScaleFactor) : mIrradiance(mIrradiance),
                                                                                     mScaleFactor(mScaleFactor),
                                                                                     mDirection(Normalize(mDirection)) {}

Vector Directional::getDirection(const ShadeRec &sr) const {
    return -mDirection;
}

RGB Directional::L(const ShadeRec &sr) {
    return mScaleFactor*mIrradiance;
}

bool Directional::inShadow(const Ray &ray, const ShadeRec &sr) const {
    unsigned long num_obj=sr.scene.getPrimitiveNum();
    double t;
    for(int i=0;i<num_obj;++i){
        if(sr.scene.getPrimitive(i)->castShadow()&&sr.scene.getPrimitive(i)->shadowHit(ray,t)){

            return true;
        }
    }
    return false;
}
