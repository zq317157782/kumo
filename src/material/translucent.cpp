/*
 * translucent.cpp
 *
 *  Created on: 2016年8月4日
 *      Author: zhuqian
 */
#include <scene.h>
#include "translucent.h"
#include "mirror.h"
#include "reflection.h"
#include "texture.h"

BSDF *Translucent::GetBSDF(const DifferentialGeometry &dgGeom,
				const DifferentialGeometry &dgShading, MemoryArena &arena) const{
	 Float ior = 1.5f;//材质的折射率
	 DifferentialGeometry dgs=dgShading;
	 BSDF *bsdf = BSDF_ALLOC(arena, BSDF)(dgs, dgGeom.nn, ior);
	 RGB r = mReflect->Evaluate(dgs);//反射系数
	 RGB t = mTransmit->Evaluate(dgs);//折射系数
	 if (r.IsBlack() && t.IsBlack()) return bsdf;
	 RGB kd = mKd->Evaluate(dgs);//漫反射折射系数
	 if (!kd.IsBlack()) {//有漫反射成分
	        if (!r.IsBlack()) bsdf->Add(BSDF_ALLOC(arena, Lambertian)(r * kd));
	        if (!t.IsBlack()) bsdf->Add(BSDF_ALLOC(arena, BRDFToBTDF)(BSDF_ALLOC(arena, Lambertian)(t * kd)));
	 }
	 RGB ks = mKs->Evaluate(dgs);//镜面反射系数
	 if (!ks.IsBlack()) {
	         Float rough = mRoughness->Evaluate(dgs);
	         //反射 基于微平面
	         if (!r.IsBlack()) {
	             Fresnel *fresnel = BSDF_ALLOC(arena, FresnelDielectric)(ior, 1.f);
	             bsdf->Add(BSDF_ALLOC(arena, Microfacet)(r * ks, fresnel,
	                 BSDF_ALLOC(arena, Blinn)(1.0f / rough)));
	         }
	         //折射 基于微平面
	         if (!t.IsBlack()) {
	             Fresnel *fresnel = BSDF_ALLOC(arena, FresnelDielectric)(ior, 1.f);
	             bsdf->Add(BSDF_ALLOC(arena, BRDFToBTDF)(BSDF_ALLOC(arena, Microfacet)(t * ks, fresnel,
	                 BSDF_ALLOC(arena, Blinn)(1.0f / rough))));
	         }
	     }
	 return bsdf;
}
