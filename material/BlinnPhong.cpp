//
// Created by 诸谦 on 15/12/29.
//

#include "BlinnPhong.h"
#include "../scene/Scene.h"


void BlinnPhong::scaleAmbientAlbedo(const float k) {
    mAmbientBrdf.scale(k);
}

void BlinnPhong::scaleDiffuseAlbedo(const float k) {
    mDiffuseBrdf.scale(k);
}

void BlinnPhong::scaleSpecularAlbedo(const float k,const float exp) {
    mSpecularBrdf.scale(k);
    mSpecularBrdf.setShiness(exp);
}

void BlinnPhong::setAlbedo(const RGB &color) {
    mAmbientBrdf.setKd(color);
    mDiffuseBrdf.setKd(color);
    mSpecularBrdf.setKs(color);
}

RGB BlinnPhong::shade(ShadeRec &sr) {
    Vector3 wo=-sr.ray.direction;
    RGB L=sr.scene.ambient()->L(sr)*mAmbientBrdf.rho(sr,wo);
    unsigned long  lignt_num=sr.scene.getLightNum();
    for(int i=0;i<lignt_num;++i){
        Vector3 wi=sr.scene.getLight(i)->getDirection(sr);
        double ndotl=sr.normal.dot(wi);
        if(ndotl>0){
            bool inShadow=false;
            if(sr.scene.getLight(i)->castShadow()){
                Ray shadowRay(sr.hitPoint,wi);
                inShadow=sr.scene.getLight(i)->inShadow(shadowRay,sr);
            }
            if(!inShadow){
                L=L+sr.scene.getLight(i)->L(sr)*(mDiffuseBrdf.f(sr,wi,wo)+mSpecularBrdf.f(sr,wi,wo))*ndotl;
            }
        }
    }
    return L;
}

RGB BlinnPhong::areaLightShade(ShadeRec &sr) {
    Vector3 wo=-sr.ray.direction;
    RGB L=sr.scene.ambient()->L(sr)*mAmbientBrdf.rho(sr,wo);
    unsigned long lignt_num=sr.scene.getLightNum();
    for(int i=0;i<lignt_num;++i){
        Vector3 wi=sr.scene.getLight(i)->getDirection(sr);
        double ndotl=sr.normal.dot(wi);
        if(ndotl>0){
            bool inShadow=false;
            if(sr.scene.getLight(i)->castShadow()){
                Ray shadowRay(sr.hitPoint,wi);
                inShadow=sr.scene.getLight(i)->inShadow(shadowRay,sr);
            }
            if(!inShadow){
                L=L+sr.scene.getLight(i)->L(sr)*(mDiffuseBrdf.f(sr,wi,wo)+mSpecularBrdf.f(sr,wi,wo))*sr.scene.getLight(i)->G(sr)*ndotl/sr.scene.getLight(i)->pdf(sr);
            }
        }
    }
    return L;
}
