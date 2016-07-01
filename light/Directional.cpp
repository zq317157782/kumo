//
// Created by 诸谦 on 15/12/27.
//

#include "Directional.h"

#include <Scene.h>

Directional::Directional(const RGB &mIrradiance, const Vector &mDirection,const float mScaleFactor) : mIrradiance(mIrradiance),
                                                                                     mScaleFactor(mScaleFactor),
                                                                                     mDirection(Normalize(mDirection)) {}

Vector Directional::getDirection(const Intersection &sr) const {
    return -mDirection;
}

RGB Directional::L(const Intersection &sr) {
    return mScaleFactor*mIrradiance;
}

bool Directional::inShadow(const Ray &ray, const Intersection &sr) const {
    unsigned long num_obj=sr.scene.getPrimitiveNum();
    double t;
    for(int i=0;i<num_obj;++i){
        if(sr.scene.getPrimitive(i)->GetShape()->castShadow()&&sr.scene.getPrimitive(i)->GetShape()->shadowHit(ray,t)){

            return true;
        }
    }
    return false;
}
