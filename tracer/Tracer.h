//
// Created by 诸谦 on 15/12/28.
//

#ifndef RAYTRACER_TRACER_H
#define RAYTRACER_TRACER_H

#include <memory>
#include "RGB.h"
#include "global.h"
using namespace std;
class Scene;
class Tracer {
protected:
    Scene& mScene;
public:
    Tracer(Scene& _scene);
    virtual RGB trace(const Ray&) const=0;
};


#endif //RAYTRACER_TRACER_H
