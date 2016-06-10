//
// Created by 诸谦 on 15/12/27.
//

#include "Ambient.h"



Vector3 Ambient::getDirection(const ShadeRec &sr) const {
    return Vector3(0,0,0);
}

RGB Ambient::L(const ShadeRec &sr) {
    return mScaleFactor*mIrradiance;
}

Ambient::Ambient(const RGB &mIrradiance, const float mScaleFactor):Light(),mIrradiance(mIrradiance),mScaleFactor(mScaleFactor){

}
