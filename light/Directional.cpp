//
// Created by 诸谦 on 15/12/27.
//

#include "Directional.h"

Directional::Directional(const RGB &mIrradiance, const Vector3 &mDirection, float mScaleFactor) : mIrradiance(mIrradiance),
                                                                                     mScaleFactor(mScaleFactor),
                                                                                     mDirection(mDirection) { this->mDirection.normalize();}

Vector3 Directional::getDirection(const ShadeRec &sr) const {
    return -mDirection;
}

RGB Directional::L(const ShadeRec &sr) const {
    return mScaleFactor*mIrradiance;
}
