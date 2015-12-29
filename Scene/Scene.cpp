//
// Created by 诸谦 on 15/12/27.
//

#include "Scene.h"
#include "../tracer/CommonTracer.h"

unsigned long Scene::getLightNum() const {
  return mLights.size();
}

const shared_ptr<Light> Scene::ambient() const {
 return mAmbient;
}

const shared_ptr<Light> Scene::getLight(const int index)const {
    return mLights[index];
}

Scene::Scene():mAmbient(shared_ptr<Light>(new Ambient())),background(RGB(0,0,0)),mTracer(shared_ptr<Tracer>(new CommonTracer(*this))),mPrimitives(){

}

unsigned long Scene::getPrimitiveNum() const {
    return mPrimitives.size();
}

const  shared_ptr<Primitive> Scene::getPrimitive(int index) const{
    return mPrimitives[index];
}

ShadeRec Scene::hit(const Ray &ray) {
    ShadeRec sr(*this);
    double t=999999;
    double dist=0;
    Vector3 normal;
    Material* material;

    for( std::vector< shared_ptr<Primitive>>::iterator it = mPrimitives.begin(); it != mPrimitives.end(); it++)
    {
        if((*it)->hit(ray,dist,sr) && t>dist){
            t=dist;
            sr.hitAnObject=true;
            sr.hitPoint=ray.position+ray.direction*t;
            normal=sr.normal;
            material=sr.material;
        }
    }

    if(sr.hitAnObject){
        sr.normal=normal;
        sr.distance=t;
        sr.material=material;
    }

    return (sr);
}

const Tracer* Scene::getTracer() const {
    return mTracer.get();
}

void Scene::setTracer(Tracer* ptr) {
    mTracer=shared_ptr<Tracer>(ptr);
}

void Scene::addPrimitive( shared_ptr<Primitive> primitive) {
    mPrimitives.push_back(primitive);
}

void Scene::addLight(shared_ptr<Light> light) {
    mLights.push_back(light);
}
