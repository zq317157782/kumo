//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H
#include <memory>
#include <vector>
#include "../light/Ambient.h"
#include "../primitive/Primitive.h"
#include "../tracer/Tracer.h"

using namespace std;
class Scene {
private:
    shared_ptr<Light> mAmbient;//环境光
    vector<shared_ptr<Light>> mLights;
    vector<shared_ptr<Primitive>> mPrimitives;
    shared_ptr<Tracer> mTracer;
public:

    Scene();
    unsigned long getLightNum() const;
    const shared_ptr<Light> ambient() const;
    const shared_ptr<Light> getLight(const int index) const;

    unsigned long getPrimitiveNum() const;
    const   shared_ptr<Primitive> getPrimitive(int index) const;

    void addPrimitive( shared_ptr<Primitive>);
    void addLight(shared_ptr<Light> light);

    virtual ShadeRec hit(const Ray& ray);

    RGB background;

    const Tracer * getTracer() const;
    void setTracer(Tracer*);
};


#endif //RAYTRACER_SCENE_H
