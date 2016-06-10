//
// Created by 诸谦 on 16/1/19.
//

#include <math.h>
#include <assert.h>
#include "CookTorrance.h"

double CookTorrance::_NDF(const Vector &N, const Vector &H) {
    double cosAlpha=Dot(N,H);
    double tanAlpha=tan(acos(cosAlpha));
    double a=(tanAlpha/mM);
    return 1.0/(mM*mM*cosAlpha*cosAlpha*cosAlpha*cosAlpha)*exp(-a*a);
}

double CookTorrance::_G(const Vector &N, const Vector &H, const Vector &wi, const Vector &wo) {
    double g1=1.0;
    double ndoth=Dot(N,H);
    double ndotv=Dot(N,wo);
    double vdoth=Dot(wo,H);
    double ndotl=Dot(N,wi);
    double g2=2*ndoth*ndotv/vdoth;
    double g3=2*ndoth*ndotl/vdoth;
    return min(min(g1,g2),g3);
}

RGB CookTorrance::_Fresnel(const Vector &wo, const Vector &H) {
    RGB one(1,1,1);
    RGB f0=mCSpecular*mScaleFactor;
    return f0+(one-f0)*pow(1-Dot(H,wo),5);
}

void CookTorrance::scale(float _f) {
    assert(_f>=0);
    mScaleFactor=_f;
}

void CookTorrance::setSpecularColor(const RGB &_cs) {
    mCSpecular=_cs;
}

void CookTorrance::setGlossy(const double m) {
    assert(m>=0);
    mM=m;
}

CookTorrance::CookTorrance(const RGB &specularColor, double m, float scaleFactor): mCSpecular(specularColor), mM(m), mScaleFactor(scaleFactor){

}

RGB CookTorrance::rho(const ShadeRec &sr, const Vector &wo) {
    return RGB(1,1,1);
}

RGB CookTorrance::f(const ShadeRec &sr, const Vector &wi, const Vector &wo) {
    Vector half=Normalize(wi + wo);
    double ndotv=Dot(sr.normal,wo);
    double ndotl=Dot(sr.normal,wi);
    return (_Fresnel(wi,half)/M_PI)*(_NDF(sr.normal,half)*_G(sr.normal,half,wi,wo)/(ndotv*ndotl));
}
