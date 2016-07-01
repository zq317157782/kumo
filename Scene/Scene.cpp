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

Reference<Primitive> Scene::getPrimitiveByID(unsigned int id) const{
	for(int i=0;i<mPrimitives.size();++i){
		if(mPrimitives[i]->primitiveID==id){
				return mPrimitives[i];
		}
	}

	return nullptr;
}

bool Scene::hit(const Ray &ray,ShadeRec* sr) {
	bool ret=false;
	for(std::vector<Reference<Primitive>>::iterator it = mPrimitives.begin(); it != mPrimitives.end(); it++)
    {
        if((*it)->CanIntersect()&&(*it)->Intersect(ray,sr)){
        	ray.maxT=sr->distance;
            //sr.hitAnObject=true;
            sr->hitPoint=sr->dg.p;
            ret=true;
        }
    }
    return ret;
}

void Scene::addPrimitive(const Reference<Primitive> primitive) {
    mPrimitives.push_back(primitive);
}

void Scene::addLight(const Reference<Light> light) {
    mLights.push_back(light);
}
