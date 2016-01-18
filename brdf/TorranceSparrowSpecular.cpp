//
// Created by 诸谦 on 15/12/29.
//

#include <math.h>
#include <assert.h>
#include "TorranceSparrowSpecular.h"

RGB TorranceSparrowSpecular::f(const ShadeRec &sr, const Vector3 &wi, const Vector3 &wo) {
    Vector3 half=(wi+wo).normalize();
    return mAlbedo/(4*M_PI*sr.normal.dot(wi))*_NDF(sr.normal,half)*_Fresnel(wo,half)*_G(sr.normal,half,wi,wo);
}

RGB TorranceSparrowSpecular::rho(const ShadeRec &sr, const Vector3 &wo) {
    return mAlbedo*mScaleFactor;
}

double TorranceSparrowSpecular::_NDF(const Vector3 &N, const Vector3 &H) {
    double ndoth=N.dot(H);
    double ndoth2=ndoth*ndoth;
    return exp(((ndoth2)-1)/(mM*mM*ndoth2))*(1/(M_PI*mM*mM*ndoth2*ndoth2));
}

double TorranceSparrowSpecular::_G(const Vector3 &N, const Vector3 &H, const Vector3 &wi, const Vector3 &wo) {
    double g1=1.0;
    double ndoth=N.dot(H);
    double ndotv=N.dot(wo);
    double vdoth=wo.dot(H);
    double ndotl=N.dot(wi);
    double g2=2*ndoth*ndotv/vdoth;
    double g3=2*ndoth*ndotl/vdoth;
    return min(min(g1,g2),g3);
}

RGB TorranceSparrowSpecular::_Fresnel(const Vector3 &wo, const Vector3 &H) {
    RGB one(1,1,1);
    RGB f0=mAlbedo*mScaleFactor;
    return f0+(one-f0)*pow(1-H.dot(wo),5);
}



void TorranceSparrowSpecular::scaleAlbedo(float _f) {
    assert(_f>=0);
    mScaleFactor=_f;
}

void TorranceSparrowSpecular::setAlbedo(const RGB &_albedo) {
    mAlbedo=_albedo;
}

void TorranceSparrowSpecular::setGlossy(const double m) {
    assert(m>=0);
    mM=m;
}

TorranceSparrowSpecular::TorranceSparrowSpecular(const RGB &mAlbedo, double mM, float mScaleFactor):mAlbedo(mAlbedo),mM(mM),mScaleFactor(mScaleFactor){

}
