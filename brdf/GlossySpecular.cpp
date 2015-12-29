//
// Created by 诸谦 on 15/12/25.
//

#include <assert.h>
#include <math.h>
#include "GlossySpecular.h"

GlossySpecular::GlossySpecular(const RGB &_albedo, float _exp, float _scaleFactor)
        :mAlbedo(_albedo),mExp(_exp),mScaleFactor(_scaleFactor)
{
}


void GlossySpecular::scaleAlbedo(float _f) {
    assert(_f>=0);
    mScaleFactor=_f;
}

void GlossySpecular::setAlbedo(const RGB &_albedo) {
    mAlbedo=_albedo;
}

RGB GlossySpecular::f(const ShadeRec &sr, const Vector3 &wi, const Vector3 &wo) {
    double ndotl=sr.normal.dot(wi);
    Vector3 r=-wi+sr.normal*ndotl*2;//求得反射向量
    double rdotv=r.dot(wo);
    RGB L(0,0,0);
    if(rdotv>0)
        L=mAlbedo*mScaleFactor*pow(rdotv,mExp);//计算高光
    return L;
}

RGB GlossySpecular::rho(const ShadeRec &sr, const Vector3 &wo) {
    return RGB(0,0,0);
}

void GlossySpecular::setShiness(float exp) {
    mExp=exp;
}
