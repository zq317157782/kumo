/*
 * SimpleIntegrator.h
 *
 *  Created on: 2016年7月1日
 *      Author: Administrator
 */

#ifndef INTEGRATOR_SIMPLEINTEGRATOR_H_
#define INTEGRATOR_SIMPLEINTEGRATOR_H_
#include "global.h"
#include "integrator.h"

class SimpleIntegrator :public SurfaceIntegrator{
public:
	SimpleIntegrator();
	virtual ~SimpleIntegrator();

	RGB Li(const Scene &scene, const Renderer &renderer,
		  const RayDifferential &ray, const Intersection &isect) const override;
};

#endif /* INTEGRATOR_SIMPLEINTEGRATOR_H_ */
