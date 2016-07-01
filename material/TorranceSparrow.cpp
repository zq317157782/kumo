//
// Created by 诸谦 on 15/12/29.
//

#include "TorranceSparrow.h"
#include "../scene/Scene.h"

void TorranceSparrow::scaleAmbientAlbedo(const float k) {
    mAmbientBrdf.scale(k);
}

void TorranceSparrow::scaleDiffuseAlbedo(const float k) {
    mDiffuseBrdf.scale(k);
}

void TorranceSparrow::scaleSpecularAlbedo(const float k) {
    mSpecularBrdf.scaleAlbedo(k);
}

void TorranceSparrow::setAlbedo(const RGB& color,const double m) {
    mAmbientBrdf.setKd(color);
    mDiffuseBrdf.setKd(color);
    mSpecularBrdf.setAlbedo(color);
    mSpecularBrdf.setGlossy(m);
}

RGB TorranceSparrow::shade(Intersection &sr) {
    Vector wo=-sr.ray.d;
    RGB L=sr.scene.ambient()->L(sr)*mAmbientBrdf.rho(sr,wo);
    unsigned long  lignt_num=sr.scene.getLightNum();
    for(int i=0;i<lignt_num;++i){
        Vector wi=sr.scene.getLight(i)->getDirection(sr);
        double ndotl=Dot(sr.normal,wi);
        if(ndotl>0){
            bool inShadow=false;
            if(sr.scene.getLight(i)->castShadow()){
                Ray shadowRay(sr.hitPoint,wi,0);
                inShadow=sr.scene.getLight(i)->inShadow(shadowRay,sr);
            }
            if(!inShadow){
                L=L+sr.scene.getLight(i)->L(sr)*(mDiffuseBrdf.f(sr,wi,wo)+mSpecularBrdf.f(sr,wi,wo))*ndotl;
            }
        }
    }
    return L;
}

RGB TorranceSparrow::areaLightShade(Intersection &sr) {
    Vector wo=-sr.ray.d;
    RGB L=sr.scene.ambient()->L(sr)*mAmbientBrdf.rho(sr,wo);
    unsigned long lignt_num=sr.scene.getLightNum();
    for(int i=0;i<lignt_num;++i){
        Vector wi=sr.scene.getLight(i)->getDirection(sr);
        double ndotl=Dot(sr.normal,wi);
        if(ndotl>0){
            bool inShadow=false;
            if(sr.scene.getLight(i)->castShadow()){
                Ray shadowRay(sr.hitPoint,wi,0);
                inShadow=sr.scene.getLight(i)->inShadow(shadowRay,sr);
            }
            if(!inShadow){
                L=L+sr.scene.getLight(i)->L(sr)*(mDiffuseBrdf.f(sr,wi,wo)+mSpecularBrdf.f(sr,wi,wo))*sr.scene.getLight(i)->G(sr)*ndotl/sr.scene.getLight(i)->pdf(sr);
            }
        }
    }
    return L;
}