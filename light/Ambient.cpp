//
// Created by 诸谦 on 15/12/27.
//

#include "Ambient.h"
#include "global.h"


Vector Ambient::getDirection(const Intersection &sr) const {
    return Vector(0,0,0);
}

RGB Ambient::L(const Intersection &sr) {
    return mScaleFactor*mIrradiance;
}

Ambient::Ambient(const RGB &mIrradiance, const float mScaleFactor):Light(),mIrradiance(mIrradiance),mScaleFactor(mScaleFactor){

}
