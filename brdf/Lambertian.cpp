//
// Created by 诸谦 on 15/12/25.
//

#include <assert.h>
#include <math.h>
#include "Lambertian.h"

Lambertian::Lambertian(const RGB &_albedo, float _scaleFactor)
        : mAlbedo(_albedo), mScaleFactor(_scaleFactor)
{ }

void Lambertian::scaleAlbedo(float _f) {
    assert(_f>=0);
    mScaleFactor=_f;
}

void Lambertian::setAlbedo(const RGB &_albedo) {
    mAlbedo=_albedo;
}

RGB Lambertian::f(const ShadeRec &sr, const Vector3 &wi, const Vector3 &wo) {
    return mAlbedo*mScaleFactor*(1.0/M_PI);
}

RGB Lambertian::rho(const ShadeRec &sr, const Vector3 &wo) {
    return mAlbedo*mScaleFactor;
}
