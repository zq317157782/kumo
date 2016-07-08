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
    Reference<Light> mAmbient;//环境光
    vector< Reference<Light>> mLights;
    vector<Reference<Primitive>> mPrimitives;
public:

    Scene();
    unsigned long getLightNum() const;
    Reference<Light>  ambient() const;
    Reference<Light> getLight(const int index) const;

    unsigned long getPrimitiveNum() const;
    Reference<Primitive> getPrimitive(int index) const;
    Reference<Primitive> getPrimitiveByID(unsigned int id) const;

    void addPrimitive(Primitive* s);
    void addLight(Light* light);

    virtual bool hit(const Ray& ray,Intersection* sr) const;

    RGB background;

    virtual ~Scene(){}
};


#endif //RAYTRACER_SCENE_H
