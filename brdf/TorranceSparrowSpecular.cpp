//
// Created by 诸谦 on 15/12/29.
//

#include <math.h>
#include <assert.h>
#include "TorranceSparrowSpecular.h"

RGB TorranceSparrowSpecular::f(const Intersection &sr, const Vector &wi, const Vector &wo) {
    Vector half=Normalize(wi + wo);
    return mAlbedo/(4*M_PI*Dot(sr.normal,wi))*_NDF(sr.normal,half)*_Fresnel(wo,half)*_G(sr.normal,half,wi,wo);
}

RGB TorranceSparrowSpecular::rho(const Intersection &sr, const Vector &wo) {
    return RGB(1,1,1);
}

double TorranceSparrowSpecular::_NDF(const Vector &N, const Vector &H) {
    double ndoth=Dot(N,H);
    double ndoth2=ndoth*ndoth;
    return exp(((ndoth2)-1)/(mM*mM*ndoth2))*(1/(M_PI*mM*mM*ndoth2*ndoth2));
}

double TorranceSparrowSpecular::_G(const Vector &N, const Vector &H, const Vector &wi, const Vector &wo) {
    double g1=1.0;
    double ndoth=Dot(N,H);
    double ndotv=Dot(N,wo);
    double vdoth=Dot(wo,H);
    double ndotl=Dot(N,wi);
    double g2=2*ndoth*ndotv/vdoth;
    double g3=2*ndoth*ndotl/vdoth;
    return min(min(g1,g2),g3);
}

RGB TorranceSparrowSpecular::_Fresnel(const Vector &wo, const Vector &H) {
    RGB one(1,1,1);
    RGB f0=mAlbedo*mScaleFactor;
    return f0+(one-f0)*pow(1-Dot(H,wo),5);
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
