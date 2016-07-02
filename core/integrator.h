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
	//todo PBRT在这里还有关于采样点和内存分配的参数
	   virtual RGB Li(const Scene *scene, const Renderer *renderer,
	        const RayDifferential &ray, const Intersection &isect) const = 0;
	   virtual ~SurfaceIntegrator(){}
};
#endif //RAYTRACER_INTEGRATOR_H
