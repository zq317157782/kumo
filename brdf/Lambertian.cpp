//
// Created by 诸谦 on 15/12/25.
//

#include <assert.h>
#include <math.h>
#include "Lambertian.h"

OldLambertian::OldLambertian(const RGB &_albedo, float _scaleFactor)
        : mKd(_albedo), mScaleFactor(_scaleFactor)
{ }

void OldLambertian::scale(float _f) {
    assert(_f>=0);
    mScaleFactor=_f;
}

void OldLambertian::setKd(const RGB &_kd) {
    mKd = _kd;
}

RGB OldLambertian::f(const Intersection &sr, const Vector &wi, const Vector &wo) {
    Vector half=Normalize(wi + wo);
//    return mKd * mScaleFactor * (1.0 / M_PI)*(RGB(1,1,1)-_Fresnel(wi,half));
    return mKd * mScaleFactor * (1.0 / M_PI);
}

RGB OldLambertian::rho(const Intersection &sr, const Vector &wo) {
    return mKd * mScaleFactor;
}

