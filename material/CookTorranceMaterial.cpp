//
// Created by 诸谦 on 16/1/19.
//

#include "CookTorranceMaterial.h"
#include "../scene/Scene.h"

void CookTorranceMaterial::setSurfaceColor(const RGB &color, const double m) {
    mCookTorranceBrdf.setSpecularColor(color);
    mCookTorranceBrdf.setGlossy(m);
}

RGB CookTorranceMaterial::shade(Intersection &sr) {
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
                L=L+ sr.scene.getLight(i)->L(sr) * (mCookTorranceBrdf.f(sr, wi, wo)) * ndotl;
            }
        }
    }
    return L;
}

RGB CookTorranceMaterial::areaLightShade(Intersection &sr) {
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
                L=L+ sr.scene.getLight(i)->L(sr) * (mCookTorranceBrdf.f(sr, wi, wo)) * sr.scene.getLight(i)->G(sr) * ndotl / sr.scene.getLight(i)->pdf(sr);
            }
        }
    }
    return L;
}

void CookTorranceMaterial::setAmbientColor(const RGB &color) {
    mAmbientBrdf.setKd(color);
}
