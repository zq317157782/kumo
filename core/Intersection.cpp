//
// Created by 诸谦 on 15/12/25.
//

#include <Intersection.h>
#include "global.h"
#include "primitive.h"

BSDF *Intersection::GetBSDF(const RayDifferential &ray, MemoryArena &arena) const{
	dg.ComputeDifferentials(ray);
	BSDF* bsdf=primitive->GetBSDF(dg,ObjectToWorld,arena);
	return bsdf;
}
