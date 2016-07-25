/*
 * metal.cpp
 *
 *  Created on: 2016年7月15日
 *      Author: Administrator
 */

#include "metal.h"
#include <Scene.h>
#include "reflection.h"
#include "texture.h"
Metal::Metal(const Reference<Texture<RGB>>& reflectance, const Reference<Texture<RGB>>& e, const Reference<Texture<RGB>>& kk,
		MicrofacetDistribution* dis) {
	mReflectance=reflectance;
	mEta=e;
	mAbsorb=kk;
	mDistribution=dis;
}

//RGB Metal::shade(const Intersection &sr) {
//
//	Vector nn = sr.normal;
//	Vector sn = Normalize(sr.dg.dpdu);
//	Vector tn = Cross(nn, sn);
//
//	Vector wo = -sr.ray.d;
//	Point hitPoint = sr.ray(sr.distance);
//	Vector wi;
//	float pdf;
//	VisibilityTester vt;
//	RGB L(0, 0, 0);
//	LightSample ls;
//	unsigned long lignt_num = sr.scene.getLightNum();
//	for (unsigned long i = 0; i < lignt_num; ++i) {
//		RGB l = sr.scene.getLight(i)->Sample_L(hitPoint, 0, ls, &wi, &pdf, &vt);
//		if (!vt.Unoccluded(&sr.scene))
//			continue;
//		double ndotl = Dot(sr.normal, wi);
//		if (ndotl > 0) {
//			wo = Vector(Dot(sn, wo), Dot(tn, wo), Dot(nn, wo));//转换到反射坐标系下
//			wi = Vector(Dot(sn, wi), Dot(tn, wi), Dot(nn, wi));
//
//			//cout<<"brdf:"<<mBrdf->f(wi,wo).r<<"brdf:"<<mBrdf->f(wi,wo).g<<"brdf:"<<mBrdf->f(wi,wo).b<<endl;
//			L = L + l * mBrdf->f(wi, wo) * ndotl;  //mDiffuseBrdf.f(sr,wi,wo)
//		}
//	}
//	return L;
//}


BSDF* Metal::GetBSDF(const DifferentialGeometry &dgGeom,
			const DifferentialGeometry &dgShading, MemoryArena &arena) const{
//TODO 还没有实现
	BSDF *bsdf=BSDF_ALLOC(arena,BSDF)(dgShading,dgGeom.nn);
	FresnelConductor*fresnel=BSDF_ALLOC(arena,FresnelConductor)(mEta->Evaluate(dgShading),mAbsorb->Evaluate(dgShading));
	bsdf->Add(BSDF_ALLOC(arena,Microfacet)(mReflectance->Evaluate(dgShading),fresnel,mDistribution));
	return bsdf;
}
//FresnelConductor


