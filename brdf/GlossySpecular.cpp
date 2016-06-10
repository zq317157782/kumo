//
// Created by 诸谦 on 15/12/25.
//

#include <assert.h>
#include <math.h>
#include "GlossySpecular.h"

GlossySpecular::GlossySpecular(const RGB &_albedo, float _exp, float _scaleFactor)
        : mKs(_albedo), mShiness(_exp), mScaleFactor(_scaleFactor)
{
}


void GlossySpecular::scale(float _f) {
    assert(_f>=0);
    mScaleFactor=_f;
}

void GlossySpecular::setKs(const RGB &_ks) {
    mKs = _ks;
}

RGB GlossySpecular::f(const ShadeRec &sr, const Vector &wi, const Vector &wo) {
    double ndotl=Dot(sr.normal,wi);
    Vector r= -wi + sr.normal * ndotl * 2;//求得反射向量
    double rdotv=Dot(r,wo);
    RGB L(0,0,0);
    if(rdotv>0)
        L= mKs * mScaleFactor * pow(rdotv, mShiness);//计算高光
    return L;
}

RGB GlossySpecular::rho(const ShadeRec &sr, const Vector &wo) {
    return mKs * mScaleFactor*(2*M_PI/(mShiness+2));
}

void GlossySpecular::setShiness(float _shiness) {
    mShiness = _shiness;
}
