//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H
#include "light.h"
#include "primitive.h"
#include "memory.h"

class Scene {
private:
	std::vector<Light*> mLights;
    Primitive* aggregate;//加速器结构
    BBox mBound;
public:

    Scene(Primitive* aggr,const std::vector<Light*>& light);
    unsigned long getLightNum() const;
    Light* getLight(const int index) const;

    virtual bool Intersect(const Ray& ray,Intersection* sr) const;
    virtual bool IntersectP(const Ray& ray) const;

    RGB background;

    virtual ~Scene(){}

    const BBox &WorldBound() const;
};


#endif //RAYTRACER_SCENE_H
