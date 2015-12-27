//
// Created by 诸谦 on 15/12/27.
//

#include "PointLight.h"

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
