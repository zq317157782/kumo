//
// Created by 诸谦 on 16/1/30.
//

#include "AmbientOccluder.h"
#include "../sampler/MultiJitteredSampler.h"
#include "../scene/Scene.h"

AmbientOccluder::AmbientOccluder(const RGB &mIrradiance, const float mScaleFactor, const float occuluderFactor):Light(),mIrradiance(mIrradiance),mScaleFactor(mScaleFactor),mOcculuderFactor(occuluderFactor){
    setSamper(new MultiJitteredSampler());
}

RGB AmbientOccluder::L(const ShadeRec &sr){
    w=sr.normal;
    v=w.cross(Vector3(0.0072,1.0,0.0034));
    v.normalize();
    u=v.cross(w);
    Ray shadow_ray;
    shadow_ray.position=sr.hitPoint;
    shadow_ray.direction=getDirection(sr);

    //光线被遮挡的情况下
    if(inShadow(shadow_ray,sr)){
        return mOcculuderFactor*mIrradiance*mScaleFactor;
    }//光线没有被遮挡的情况
    else
        return mIrradiance*mScaleFactor;
}

void AmbientOccluder::setSamper(Sampler*sampler) {
    mSampler=sampler;
}


Vector3 AmbientOccluder::getDirection(const ShadeRec &sr) const {
    Vector3 point=mSampler->sampleHemi();
    return u*point.x+v*point.y+w*point.z;
}

bool AmbientOccluder::inShadow(const Ray &ray, const ShadeRec &sr) const {

    unsigned long num_obj= sr.scene.getPrimitiveNum();
    double t;
    for(int i=0;i<num_obj;++i){
        Primitive* p=sr.scene.getPrimitive(i);
        if(p->castShadow()&&p->shadowHit(ray,t)){
            return true;
        }
    }
    return false;
}