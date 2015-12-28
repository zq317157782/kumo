//
// Created by 诸谦 on 15/12/28.
//

#ifndef RAYTRACER_COMMONTRACER_H
#define RAYTRACER_COMMONTRACER_H


#include "Tracer.h"

class CommonTracer : public Tracer{

public:
    CommonTracer(Scene& _scene);

    virtual RGB trace(const Ray &ray) const;
};


#endif //RAYTRACER_COMMONTRACER_H
