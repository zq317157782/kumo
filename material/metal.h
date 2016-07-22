/*
 * metal.h
 *
 *  Created on: 2016年7月15日
 *      Author: Administrator
 */

#ifndef MATERIAL_METAL_H_
#define MATERIAL_METAL_H_

#include <material.h>
#include "global.h"

class Metal: public Material {
private:
	Microfacet* mBrdf;
	FresnelConductor* mFresnel;
public:
	Metal(const RGB& reflectance, const RGB& e, const RGB& kk,
			MicrofacetDistribution* dis);
	virtual RGB shade(const Intersection& sr) override;
	virtual ~Metal(){}
	virtual BSDF *GetBSDF(const DifferentialGeometry &dgGeom,
			const DifferentialGeometry &dgShading, MemoryArena &arena) const
					override;
};

#endif /* MATERIAL_METAL_H_ */
