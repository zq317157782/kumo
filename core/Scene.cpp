//
// Created by 诸谦 on 15/12/27.
//

#include <Scene.h>

#include "diffgeom.h"

unsigned long Scene::getLightNum() const {
	return mLights.size();
}



Light* Scene::getLight(const int index) const {
	return mLights[index];
}

Scene::Scene() :background(RGB(0, 0, 0)), mPrimitives() {

}

unsigned long Scene::getPrimitiveNum() const {
	return mPrimitives.size();
}

Reference<Primitive> Scene::getPrimitive(int index) const {
	return mPrimitives[index];
}

Reference<Primitive> Scene::getPrimitiveByID(unsigned int id) const {
	for (int i = 0; i < mPrimitives.size(); ++i) {
		if (mPrimitives[i]->primitiveID == id) {
			return mPrimitives[i];
		}
	}
	return nullptr;
}

bool Scene::Intersect(const Ray &ray, Intersection* sr) const {
	bool ret = false;
	for (auto it = mPrimitives.begin(); it != mPrimitives.end(); it++) {
		if ((*it)->CanIntersect()) {
			if ((*it)->Intersect(ray, sr)) {
				ray.maxT = sr->distance;
				ret = true;
			}
		}
	}
	return ret;
}

bool Scene::IntersectP(const Ray& ray) const {
	for (auto i =0; i < mPrimitives.size(); ++i) {
			if ((mPrimitives[i])->CanIntersect()) {
				if ((mPrimitives[i])->IntersectP(ray)) {
					return true;
				}
			}
		}
		return false;
}

void Scene::addPrimitive(Primitive* primitive) {
	if (!primitive->CanIntersect()) {
		vector<Reference<Primitive>> refine;
		primitive->Refine(refine);
		for (int i = 0; i < refine.size(); ++i) {
			mPrimitives.push_back(refine[i]);
		}
	}
	mPrimitives.push_back(primitive);
}

void Scene::addLight(Light* light) {
	mLights.push_back(light);
}
