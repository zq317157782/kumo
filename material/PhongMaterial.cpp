//
// Created by 诸谦 on 15/12/27.
//

#include "PhongMaterial.h"

#include <Scene.h>

//void Phong::scaleAmbientAlbedo(const float k) {
//    mAmbientBrdf.scale(k);
//}
//
//void Phong::scaleDiffuseAlbedo(const float k) {
//    mDiffuseBrdf.scale(k);
//}
//
//void Phong::scaleSpecularAlbedo(const float k) {
//    mSpecularBrdf.scale(k);
//}
//
//void Phong::setAlbedo(const RGB &color,const float exp) {
//    mAmbientBrdf.setKd(color);
//    mDiffuseBrdf.setKd(color);
//    mSpecularBrdf.setKs(color);
//    mSpecularBrdf.setShiness(exp);
//}

RGB PhongMaterial::shade(Intersection &sr) {
    Vector wo=-sr.ray.d;
    RGB L=sr.scene.ambient()->L(sr)*mAmbientBrdf.rho(sr,wo);
    unsigned long  lignt_num=sr.scene.getLightNum();
    for(int i=0;i<lignt_num;++i){
        Vector wi=sr.scene.getLight(i)->getDirection(sr);
        double ndotl= Dot(sr.normal,wi);
        if(ndotl>0){
            bool inShadow=false;
            if(sr.scene.getLight(i)->castShadow()){
                Ray shadowRay(sr.dg.p,wi,0);
                inShadow=sr.scene.getLight(i)->inShadow(shadowRay,sr);
            }
            if(!inShadow){
                L=L+sr.scene.getLight(i)->L(sr)*(mDiffuseBrdf.f(sr,wi,wo)+mSpecularBrdf.f(sr,wi,wo))*ndotl;
            }
        }
    }
    return L;
}

RGB PhongMaterial::areaLightShade(Intersection &sr) {
    Vector wo=-sr.ray.d;
    RGB L=sr.scene.ambient()->L(sr)*mAmbientBrdf.rho(sr,wo);
    unsigned long lignt_num=sr.scene.getLightNum();
    for(int i=0;i<lignt_num;++i){
        Vector wi=sr.scene.getLight(i)->getDirection(sr);
        double ndotl=Dot(sr.normal,wi);
        if(ndotl>0){
            bool inShadow=false;
            if(sr.scene.getLight(i)->castShadow()){
                Ray shadowRay(sr.dg.p,wi,0);
                inShadow=sr.scene.getLight(i)->inShadow(shadowRay,sr);
            }
            if(!inShadow){
                L=L+sr.scene.getLight(i)->L(sr)*(mDiffuseBrdf.f(sr,wi,wo)+mSpecularBrdf.f(sr,wi,wo))*sr.scene.getLight(i)->G(sr)*ndotl/sr.scene.getLight(i)->pdf(sr);
            }
        }
    }
    return L;
}

void PhongMaterial::setSurfaceColor(const RGB &color, const float shiness) {
    mSpecularBrdf.setKs(color);
    mSpecularBrdf.setShiness(shiness);
}

void PhongMaterial::setAmbientColor(const RGB &color) {
    mAmbientBrdf.setKd(color);
}

void PhongMaterial::setDiffuseColor(const RGB &color) {
    mDiffuseBrdf.setKd(color);
}
