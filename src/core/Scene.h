//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H
#include "light.h"
#include "primitive.h"
#include "memory.h"


using namespace std;
class Scene {
private:
    vector<Light*> mLights;
    //vector<Reference<Primitive>> mPrimitives;
    Primitive* aggregate;//加速器结构
public:

    Scene(Primitive* aggr,const vector<Light*>& light);
    unsigned long getLightNum() const;
    Light* getLight(const int index) const;

//    unsigned long getPrimitiveNum() const;
//    Reference<Primitive> getPrimitive(int index) const;
//    Reference<Primitive> getPrimitiveByID(unsigned int id) const;

    //void addPrimitive(Primitive* s);
    //void addLight(Light* light);

    virtual bool Intersect(const Ray& ray,Intersection* sr) const;
    virtual bool IntersectP(const Ray& ray) const;

    RGB background;

    virtual ~Scene(){}
};


#endif //RAYTRACER_SCENE_H
