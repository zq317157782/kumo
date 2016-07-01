//
// Created by 诸谦 on 15/12/27.
//

#include "Matte.h"
#include "../scene/Scene.h"

Matte::Matte(const RGB& _albedo):
        mAmbientBrdf(OldLambertian(_albedo)),
        mDiffuseBrdf(OldLambertian(_albedo)){

}

void Matte::scaleAmbientAlbedo(const float k) {
    mAmbientBrdf.scale(k);
}

void Matte::scaleDiffuseAlbedo(const float k) {
    mDiffuseBrdf.scale(k);
}

void Matte::setAlbedo(const RGB &color) {
    mAmbientBrdf.setKd(color);
    mDiffuseBrdf.setKd(color);
}

RGB Matte::shade(ShadeRec &sr) {
    Vector wo=-sr.ray.d;
    RGB L=sr.scene.ambient()->L(sr)*mAmbientBrdf.rho(sr,wo);
    unsigned long lignt_num=sr.scene.getLightNum();
    for(unsigned long i=0;i<lignt_num;++i){
        Vector wi=sr.scene.getLight(i)->getDirection(sr);
        double ndotl=Dot(sr.normal,wi);
        if(ndotl>0){
            L=L+sr.scene.getLight(i)->L(sr)*mDiffuseBrdf.f(sr,wi,wo)*ndotl;
        }
    }
    return L;
}

RGB Matte::areaLightShade(ShadeRec &sr) {
    return RGB(0,0,0);
}
