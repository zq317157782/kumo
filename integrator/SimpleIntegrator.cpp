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
#include "reflection.h"
SimpleIntegrator::SimpleIntegrator() {

}

SimpleIntegrator::~SimpleIntegrator() {

}

RGB SimpleIntegrator::Li(const Scene *scene, const Renderer *renderer,
		const RayDifferential &ray,const Sample *sample, const Intersection &isect, Random &rnd,
		MemoryArena& arena) const {

	Reference<Primitive> primitive = scene->getPrimitiveByID(isect.primitiveID);
	BSDF* bsdf = isect.GetBSDF(ray, arena);

	Vector wo = -ray.d;
	//`	`	cout<<Dot(wo,isect.normal)<<endl;
	Point hitPoint = ray(isect.distance);
	Vector wi;
	float pdf;
	VisibilityTester vt;
	RGB L(0, 0, 0);
	L=isect.Le(wo);
	LightSample ls;
	unsigned long lignt_num = scene->getLightNum();
	for (unsigned long i = 0; i < lignt_num; ++i) {
		RGB l = scene->getLight(i)->Sample_L(hitPoint, 0, ls, &wi, &pdf, &vt);
		//cout<<"wiWorld:"<<wi.x<<wi.y<<wi.y<<endl;
		if (!vt.Unoccluded(scene))
			continue;
		double ndotl = Dot(isect.normal, wi);
		if (ndotl > 0) {
			L = L + l * bsdf->f(wo, wi) * ndotl;  //mDiffuseBrdf.f(sr,wi,wo)
			//cout<<L.r<<" "<<L.g<<" "<<L.b<<" "<<endl;
		}
	}
	return L;
}

/*
 *
 * 	Vector nn = sr.normal;
 Vector sn = Normalize(sr.dg.dpdu);
 Vector tn = Cross(nn, sn);

 Vector wo = -sr.ray.d;
 Point hitPoint = sr.ray(sr.distance);
 Vector wi;
 float pdf;
 VisibilityTester vt;
 RGB L(0, 0, 0);
 LightSample ls;
 unsigned long lignt_num = sr.scene.getLightNum();
 for (unsigned long i = 0; i < lignt_num; ++i) {
 RGB l = sr.scene.getLight(i)->Sample_L(hitPoint, 0, ls, &wi, &pdf, &vt);
 if (!vt.Unoccluded(&sr.scene))
 continue;
 double ndotl = Dot(sr.normal, wi);
 if (ndotl > 0) {
 wo = Vector(Dot(sn, wo), Dot(tn, wo), Dot(nn, wo));//转换到反射坐标系下
 wi = Vector(Dot(sn, wi), Dot(tn, wi), Dot(nn, wi));

 //cout<<"brdf:"<<mBrdf->f(wi,wo).r<<"brdf:"<<mBrdf->f(wi,wo).g<<"brdf:"<<mBrdf->f(wi,wo).b<<endl;
 L = L + l * mBrdf->f(wi, wo) * ndotl;  //mDiffuseBrdf.f(sr,wi,wo)
 }
 }
 return L;
 *
 * */
