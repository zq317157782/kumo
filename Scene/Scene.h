//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H
#include <vector>
#include "light.h"
#include "Shape.h"
#include "memory.h"
#include "../tracer/Tracer.h"


using namespace std;
class Scene {
private:
    Reference<Light> mAmbient;//环境光
    vector< Reference<Light>> mLights;
    vector<Reference<Shape>> mPrimitives;
    Tracer* mTracer;
public:

    Scene();
    unsigned long getLightNum() const;
    Reference<Light>  ambient() const;
    Reference<Light> getLight(const int index) const;

    unsigned long getPrimitiveNum() const;
    Reference<Shape> getPrimitive(int index) const;

    void addPrimitive(const Reference<Shape> s);
    void addLight(const Reference<Light> light);

    virtual ShadeRec hit(const Ray& ray);

    RGB background;
};


#endif //RAYTRACER_SCENE_H
