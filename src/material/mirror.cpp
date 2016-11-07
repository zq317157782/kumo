/*
 * mirror.cpp
 *
 *  Created on: 2016年8月4日
 *      Author: zhuqian
 */
#include <scene.h>
#include "mirror.h"
#include "reflection.h"
#include "texture.h"

BSDF* Mirror::GetBSDF(const DifferentialGeometry &dgGeom,
		const DifferentialGeometry &dgShading, MemoryArena &arena) const {
	DifferentialGeometry dgs = dgShading;
	BSDF *bsdf = BSDF_ALLOC(arena,BSDF)(dgShading, dgGeom.nn);
	RGB R = mKr->Evaluate(dgs);
	if (!R.IsBlack())
		bsdf->Add(BSDF_ALLOC(arena, SpecularReflection)(R,
		BSDF_ALLOC(arena, FresnelNoOp)()));//这里使用了FresnelNoOp;
	return bsdf;
}

