/*
 * grass.cpp
 *
 *  Created on: 2016年8月23日
 *      Author: zhuqian
 */
#include "grass.h"
#include "reflection.h"
#include "texture.h"

BSDF * Grass::GetBSDF(const DifferentialGeometry &dgGeom,
				const DifferentialGeometry &dgShading, MemoryArena &arena) const{
	 DifferentialGeometry dgs;
	 dgs = dgShading;
	 Float ior = mIndex->Evaluate(dgs);//材质折射率
	 BSDF *bsdf = BSDF_ALLOC(arena, BSDF)(dgs, dgGeom.nn, ior);
	 RGB R = Clamp(mKr->Evaluate(dgs),0,1);
	 RGB T = Clamp(mKt->Evaluate(dgs),0,1);
	 if(!R.IsBlack()){
		 bsdf->Add(BSDF_ALLOC(arena, SpecularReflection)(R,BSDF_ALLOC(arena, FresnelDielectric)(1.0f, ior)));//镜面反射BRDF
	 }
	 if(!T.IsBlack()){
		 bsdf->Add(BSDF_ALLOC(arena, SpecularTransmission)(T, 1.0f, ior));
	 }
	 return bsdf;
}
