//
// Created by 诸谦 on 15/12/25.
//

#include <assert.h>
#include <math.h>
#include "Lambertian.h"

Lambertian::Lambertian(const RGB &_albedo, float _scaleFactor)
        : mKd(_albedo), mScaleFactor(_scaleFactor)
{ }

void Lambertian::scale(float _f) {
    assert(_f>=0);
    mScaleFactor=_f;
}

void Lambertian::setKd(const RGB &_kd) {
    mKd = _kd;
}

RGB Lambertian::f(const ShadeRec &sr, const Vector &wi, const Vector &wo) {
    Vector half=Normalize(wi + wo);
//    return mKd * mScaleFactor * (1.0 / M_PI)*(RGB(1,1,1)-_Fresnel(wi,half));
    return mKd * mScaleFactor * (1.0 / M_PI);
}

RGB Lambertian::rho(const ShadeRec &sr, const Vector &wo) {
    return mKd * mScaleFactor;
}

