/*
 * metal.h
 *
 *  Created on: 2016年7月15日
 *      Author: Administrator
 */

#ifndef MATERIAL_METAL_H_
#define MATERIAL_METAL_H_

#include <material.h>
#include "../core/seidennki.h"

class Metal: public Material {
private:
	Reference<Texture<RGB>> mReflectance;
	Reference<Texture<RGB>> mEta;
	Reference<Texture<RGB>> mAbsorb;
	MicrofacetDistribution* mDistribution;
public:
	Metal(const Reference<Texture<RGB>>& reflectance, const Reference<Texture<RGB>>& e, const Reference<Texture<RGB>>& kk,
			MicrofacetDistribution* dis);
	virtual ~Metal(){}
	virtual BSDF *GetBSDF(const DifferentialGeometry &dgGeom,
			const DifferentialGeometry &dgShading, MemoryArena &arena) const
					override;
};

#endif /* MATERIAL_METAL_H_ */
