//
// Created by 诸谦 on 16/1/30.
//

#include "AmbientOccluder.h"
#include "../sampler/MultiJitteredSampler.h"
#include "../scene/Scene.h"

AmbientOccluder::AmbientOccluder(const RGB &mIrradiance, const float mScaleFactor, const float occuluderFactor):Light(),mIrradiance(mIrradiance),mScaleFactor(mScaleFactor),mOcculuderFactor(occuluderFactor){
    setSamper(new MultiJitteredSampler());
}

RGB AmbientOccluder::L(const Intersection &sr){
    w=Vector(sr.normal);
    v=Normalize(Cross(w,Vector(0.0072, 1.0, 0.0034)));
    u=Cross(v,w);
    Ray shadow_ray(sr.hitPoint,getDirection(sr),0);

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


Vector AmbientOccluder::getDirection(const Intersection &sr) const {
    Vector point=mSampler->sampleHemi();
    return u*point.x+v*point.y+w*point.z;
}

bool AmbientOccluder::inShadow(const Ray &ray, const Intersection &sr) const {

//    unsigned long num_obj= sr.scene.getPrimitiveNum();
//    double t;
//    for(int i=0;i<num_obj;++i){
//        Reference<Primitive> p=sr.scene.getPrimitive(i);
//        if(p->castShadow()&&p->shadowHit(ray,t)){
//            return true;
//        }
//    }
    return false;
}
