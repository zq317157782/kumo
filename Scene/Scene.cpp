//
// Created by 诸谦 on 15/12/27.
//

#include "Scene.h"

unsigned long Scene::getLightNum() const {
  return mLights.size();
}

const shared_ptr<Light> Scene::ambient() const {
 return mAmbient;
}

const shared_ptr<Light> Scene::getLight(const int index)const {
    return mLights[index];
}

Scene::Scene(const shared_ptr<Light> &mAmbient):mAmbient(mAmbient){

}
