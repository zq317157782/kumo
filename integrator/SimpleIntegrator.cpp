/*
 * SimpleIntegrator.cpp
 *
 *  Created on: 2016年7月1日
 *      Author: Administrator
 */

#include "SimpleIntegrator.h"
#include "RGB.h"
#include "Intersection.h"
#include "Scene.h"
SimpleIntegrator::SimpleIntegrator() {


}

SimpleIntegrator::~SimpleIntegrator() {

}

RGB SimpleIntegrator::Li(const Scene *scene, const Renderer *renderer,
		  const RayDifferential &ray, const Intersection &isect,Random &rnd,MemoryArena& arena) const{
	    	Reference<Primitive> primitive= scene->getPrimitiveByID(isect.primitiveID);

	        return primitive->GetMaterial()->shade(isect);
}
