/*
 * metal.cpp
 *
 *  Created on: 2016年7月15日
 *      Author: Administrator
 */

#include <scene.h>
#include "metal.h"
#include "reflection.h"
#include "texture.h"
Metal::Metal(const Reference<Texture<RGB>>& reflectance, const Reference<Texture<RGB>>& e, const Reference<Texture<RGB>>& kk,
		MicrofacetDistribution* dis) {
	mReflectance=reflectance;
	mEta=e;
	mAbsorb=kk;
	mDistribution=dis;
}

BSDF* Metal::GetBSDF(const DifferentialGeometry &dgGeom,
			const DifferentialGeometry &dgShading, MemoryArena &arena) const{
	BSDF *bsdf=BSDF_ALLOC(arena,BSDF)(dgShading,dgGeom.nn);
	FresnelConductor*fresnel=BSDF_ALLOC(arena,FresnelConductor)(mEta->Evaluate(dgShading),mAbsorb->Evaluate(dgShading));
	bsdf->Add(BSDF_ALLOC(arena,Microfacet)(mReflectance->Evaluate(dgShading),fresnel,mDistribution));
	return bsdf;
}
//FresnelConductor



