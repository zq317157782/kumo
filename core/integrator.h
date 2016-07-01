//
// Created by 诸谦 on 16/7/1.
//

#ifndef RAYTRACER_INTEGRATOR_H
#define RAYTRACER_INTEGRATOR_H
#include "global.h"
//积分器
class Integrator{

};


class SurfaceIntegrator:public Integrator{
public:
	    virtual RGB Li(const Scene &scene, const Renderer &renderer,
	        const RayDifferential &ray, const Intersection &isect) const = 0;
};
#endif //RAYTRACER_INTEGRATOR_H
