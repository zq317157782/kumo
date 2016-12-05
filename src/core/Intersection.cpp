//
// Created by 诸谦 on 15/12/25.
//

#include <Intersection.h>
#include <kumo.h>
#include "primitive.h"
#include "light.h"

BSDF *Intersection::GetBSDF(const RayDifferential &ray, MemoryArena &arena) const{
	dg.ComputeDifferentials(ray);
	BSDF* bsdf=primitive->GetBSDF(dg,ObjectToWorld,arena);
	return bsdf;
}


RGB Intersection::Le(const Vector &wo) const{
	const AreaLight* areaLight=primitive->GetAreaLight();
	return areaLight?areaLight->L(dg.p,dg.nn,wo):RGB(0,0,0);
}
