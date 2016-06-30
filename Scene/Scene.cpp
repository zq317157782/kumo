//
// Created by 诸谦 on 15/12/27.
//

#include "Scene.h"
#include "../light/ambient.h"
#include "diffgeom.h"

unsigned long Scene::getLightNum() const {
  return mLights.size();
}

Reference<Light> Scene::ambient() const {
 return mAmbient;
}

Reference<Light> Scene::getLight(const int index)const {
    return mLights[index];
}

Scene::Scene():mAmbient(Reference<Light>(new Ambient())),background(RGB(0,0,0)),mPrimitives(){

}

unsigned long Scene::getPrimitiveNum() const {
    return mPrimitives.size();
}

Reference<Primitive> Scene::getPrimitive(int index) const{
    return mPrimitives[index];
}

ShadeRec Scene::hit(const Ray &ray) {
    ShadeRec sr(*this,ray);
    for(std::vector<Reference<Primitive>>::iterator it = mPrimitives.begin(); it != mPrimitives.end(); it++)
    {
        if((*it)->CanIntersect()&&(*it)->Intersect(ray,&sr)){
            sr.hitAnObject=true;
            sr.hitPoint=sr.dg.p;
        }
    }

//    if(sr.hitAnObject){
//        sr.normal=normal;
//        sr.distance=sr.dg.;
//        sr.material=material;
//    }

    return (sr);
}

void Scene::addPrimitive(const Reference<Primitive> primitive) {
    mPrimitives.push_back(primitive);
}

void Scene::addLight(const Reference<Light> light) {
    mLights.push_back(light);
}
