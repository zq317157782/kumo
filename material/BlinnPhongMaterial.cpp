//
// Created by 诸谦 on 15/12/29.
//

#include "BlinnPhongMaterial.h"
#include "../scene/Scene.h"


//void BlinnPhongMaterial::scaleAmbientAlbedo(const float k) {
//    mAmbientBrdf.scale(k);
//}
//
//void BlinnPhongMaterial::scaleDiffuseAlbedo(const float k) {
//    mDiffuseBrdf.scale(k);
//}
//
//void BlinnPhongMaterial::scaleSpecularAlbedo(const float k,const float exp) {
//    mSpecularBrdf.scale(k);
//    mSpecularBrdf.setShiness(exp);
//}
//
//void BlinnPhongMaterial::setAlbedo(const RGB &color) {
//    mAmbientBrdf.setKd(color);
//    mDiffuseBrdf.setKd(color);
//    mSpecularBrdf.setKs(color);
//}

RGB BlinnPhongMaterial::shade(ShadeRec &sr) {
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

RGB BlinnPhongMaterial::areaLightShade(ShadeRec &sr) {
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

void BlinnPhongMaterial::setSurfaceColor(const RGB &color, const float shiness) {
    mSpecularBrdf.setKs(color);
    mSpecularBrdf.setShiness(shiness);

}

void BlinnPhongMaterial::setAmbientColor(const RGB &color) {
    mAmbientBrdf.setKd(color);
}

void BlinnPhongMaterial::setDiffuseColor(const RGB &color) {
    mDiffuseBrdf.setKd(color);
}
