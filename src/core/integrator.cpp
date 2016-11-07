/*
 * integrator.cpp
 *
 *  Created on: 2016年7月30日
 *      Author: zhuqian
 */
#include <scene.h>
#include "integrator.h"
#include "global.h"
#include "light.h"
#include "montecarlo.h"
#include "renderer.h"
#include "sampler.h"
Distribution1D *ComputeLightSamplingCDF(const Scene *scene) {
	unsigned int nLights = scene->getLightNum();
	vector<float> lightPower(nLights, 0.0f);
	for (unsigned int i = 0; i < nLights; ++i)
		lightPower[i] = scene->getLight(i)->Power(scene).luminance();
	return new Distribution1D(&lightPower[0], nLights);
}
//TODO 这个函数中还缺少透射相关的代码
RGB EstimateDirect(const Scene* scene, const Renderer*renderer,
		MemoryArena& arena, const Light* light, const Point& p, const Normal& n,
		const Vector& wo, float rayEpsilon, const BSDF *bsdf, Random& rand,
		const LightSample& lightSample, const BSDFSample& bsdfSample,
		BxDFType flags) {
	RGB Ld(0); //最后的direct radiance;
	Vector wi;
	float lightPdf, bsdfPdf;
	VisibilityTester visibility;
	//这里采样光源分布
	RGB Li = light->Sample_L(p, rayEpsilon, lightSample, &wi, &lightPdf,
			&visibility);
	if (lightPdf > 0 && !Li.IsBlack()) {
		RGB f = bsdf->f(wo, wi, flags);
		if (!f.IsBlack() && visibility.Unoccluded(scene)) {
			if (light->IsDeltaLight()) {
				//这里不使用MIS策略
				//	return RGB(1,1,1);
				Ld += f * Li * (AbsDot(wi, n) / lightPdf);
			} else {
				//使用MIS策略
				bsdfPdf = bsdf->Pdf(wo, wi, flags);
				float weight = PowerHeuristic(1, lightPdf, 1, bsdfPdf);
				Ld += f * Li * (AbsDot(wi, n) * weight / lightPdf);
			}
		}
	}

	if (!light->IsDeltaLight()) {
		BxDFType sampledType; //这里要采样BSDF分布
		RGB f = bsdf->Sample_f(wo, &wi, bsdfSample, &bsdfPdf, flags,
				&sampledType);
		if (!f.IsBlack() && bsdfPdf > 0) {
			float weight = 1;
			//BSDF不是镜面的时候,MIS
			if (!(sampledType & BSDF_SPECULAR)) {
				lightPdf = light->Pdf(p, wi);
				if (lightPdf == 0)
					return Ld;
				//走MIS
				weight = PowerHeuristic(1, bsdfPdf, 1, lightPdf);
			}
			RGB Li(0);
			Intersection lightInc;
			RayDifferential ray(p, wi, rayEpsilon, INFINITY);
			if (scene->Intersect(ray, &lightInc)) {
				if (lightInc.primitive->GetAreaLight() == light) {
					Li = lightInc.Le(-wi);
				}
			} else {
				//TODO 这里有还没有实现的代码
			}
			if (!Li.IsBlack()) {
				//MIS
				Ld += f * Li * (AbsDot(wi, n) * weight / bsdfPdf);
			}
		}
	}
	return Ld;
}

RGB UniformSampleAllLights(const Scene *scene, const Renderer *renderer,
		MemoryArena &arena, const Point &p, const Normal &n, const Vector &wo,
		float rayEpsilon, BSDF *bsdf, const Sample *sample, Random &rand,
		const LightSampleOffsets *lightSampleOffsets,
		const BSDFSampleOffsets *bsdfSampleOffsets) {
	RGB L(0);
	for (int i = 0; i < scene->getLightNum(); ++i) {
		Light*light = scene->getLight(i);
		int numSample = lightSampleOffsets ? lightSampleOffsets[i].nSamples : 1; //获得此光源的采样样本数
		RGB Ld(0);
		for (int j = 0; j < numSample; ++j) {
			LightSample lightSample;
			BSDFSample bsdfSample;
			if (lightSampleOffsets != nullptr && bsdfSampleOffsets != nullptr) {
				lightSample = LightSample(sample, lightSampleOffsets[i], j);
				bsdfSample = BSDFSample(sample, bsdfSampleOffsets[i], j);
			} else {
				lightSample = LightSample(rand);
				bsdfSample = BSDFSample(rand);
			}
			Ld += EstimateDirect(scene, renderer, arena, light, p, n, wo,
					rayEpsilon, bsdf, rand, lightSample, bsdfSample,
					BxDFType(BSDF_ALL & ~BSDF_SPECULAR));
		}
		L += Ld / numSample;
	}
	return L;
}

RGB UniformSampleOneLight(const Scene *scene, const Renderer *renderer,
		MemoryArena &arena, const Point &p, const Normal &n, const Vector &wo,
		float rayEpsilon, BSDF *bsdf, const Sample *sample, Random &rand,
		int lightNumOffset, const LightSampleOffsets *lightSampleOffset,
		const BSDFSampleOffsets *bsdfSampleOffset) {
	int nLights = scene->getLightNum();
	if (nLights == 0)
		return RGB(0.0);
	int lightNum;
	if (lightNumOffset != -1)
		lightNum = Floor2Int(sample->oneD[lightNumOffset][0] * nLights);
	else
		lightNum = Floor2Int(rand.RandomFloat() * nLights);
	lightNum = min(lightNum, nLights - 1);
	Light *light = scene->getLight(lightNum);

	LightSample lightSample;
	BSDFSample bsdfSample;
	if (lightSampleOffset != nullptr && bsdfSampleOffset != nullptr) {
		lightSample = LightSample(sample, *lightSampleOffset, 0);
		bsdfSample = BSDFSample(sample, *bsdfSampleOffset, 0);
	} else {
		lightSample = LightSample(rand);
		bsdfSample = BSDFSample(rand);
	}
	return (float) nLights
			* EstimateDirect(scene, renderer, arena, light, p, n, wo,
					rayEpsilon, bsdf, rand, lightSample, bsdfSample,
					BxDFType(BSDF_ALL & ~BSDF_SPECULAR));
}

RGB SpecularReflect(const RayDifferential &ray, BSDF *bsdf, Random &rand,
		const Intersection &isect, const Renderer *renderer, const Scene *scene,
		const Sample *sample, MemoryArena &arena) {
	Vector wo = -ray.d; //出射方向
	Vector wi; //入射方向
	float pdf;
	const Point &p = bsdf->dgShading.p;
	const Normal &n = bsdf->dgShading.nn;
	RGB f = bsdf->Sample_f(wo, &wi, BSDFSample(rand), &pdf,
			BxDFType(BSDF_REFLECTION | BSDF_SPECULAR));
	RGB L(0);
	//cout<<"pdf:"<<pdf<<" f:"<<f.r<<f.g<<f.b<<endl;
	if (pdf > 0.0f && !f.IsBlack() && AbsDot(wi, n) != 0.0f) {
		RayDifferential r(p, wi, ray, isect.rayEpsilon);
		//PBRT P513
		if (r.hasDifferentials) {
			r.hasDifferentials = true;
			//一次逼近
			r.rxOrigin = p + isect.dg.dpdx;
			r.ryOrigin = p + isect.dg.dpdy;
			//链式法则
			Normal dndx = bsdf->dgShading.dndu * bsdf->dgShading.dudx
					+ bsdf->dgShading.dndv * bsdf->dgShading.dvdx;
			Normal dndy = bsdf->dgShading.dndu * bsdf->dgShading.dudy
					+ bsdf->dgShading.dndv * bsdf->dgShading.dvdy;
			//出射方向的差分
			Vector dwodx = -ray.rxDirection - wo, dwody = -ray.ryDirection - wo;
			//dwidx dwidy
			float dDNdx = Dot(dwodx, n) + Dot(wo, dndx);
			float dDNdy = Dot(dwody, n) + Dot(wo, dndy);
			r.rxDirection = wi - dwodx
					+ 2 * Vector(Dot(wo, n) * dndx + dDNdx * n); //w=wi+dwidx
			r.ryDirection = wi - dwody
					+ 2 * Vector(Dot(wo, n) * dndy + dDNdy * n); //w=wi+dwidy
		}

		RGB Li = renderer->Li(scene, r, sample, rand, arena);
		L = f * Li * AbsDot(wi, n) / pdf;
		//cout<<L.r<<L.g<<L.b<<endl;
	}
	return L;
}

RGB SpecularTransmit(const RayDifferential &ray, BSDF *bsdf, Random &rand,
		const Intersection &isect, const Renderer *renderer, const Scene *scene,
		const Sample *sample, MemoryArena &arena) {
	Vector wo = -ray.d; //出射方向
	Vector wi; //入射方向
	float pdf;
	const Point &p = bsdf->dgShading.p;
	const Normal &n = bsdf->dgShading.nn;
	RGB f = bsdf->Sample_f(wo, &wi, BSDFSample(rand), &pdf,
			BxDFType(BSDF_TRANSMISSION | BSDF_SPECULAR));
	RGB L(0);
	if (pdf > 0.0f && !f.IsBlack() && AbsDot(wi, n) != 0.0f) {
		RayDifferential r(p, wi, ray, isect.rayEpsilon);
		//这里要用到Fresnel折射
		if (ray.hasDifferentials) {
			r.hasDifferentials = true;
			//设置辅助射线的位置
			r.rxOrigin = p + isect.dg.dpdx;
			r.ryOrigin = p + isect.dg.dpdy;

			//材质折射率
			float eta = bsdf->eta;
			//射线方向
			Vector w = -wo;

			if (Dot(wo, n) < 0)
				eta = 1.0f / eta;
			//法线的偏导
			Normal dndx = bsdf->dgShading.dndu * bsdf->dgShading.dudx
					+ bsdf->dgShading.dndv * bsdf->dgShading.dvdx;
			Normal dndy = bsdf->dgShading.dndu * bsdf->dgShading.dudy
					+ bsdf->dgShading.dndv * bsdf->dgShading.dvdy;
			//入射射线偏导
			Vector dwodx = (-ray.rxDirection) - wo;
			Vector dwody = (-ray.ryDirection) - wo;

			float dDNdx = Dot(dwodx, n) + Dot(wo, dndx);
			float dDNdy = Dot(dwody, n) + Dot(wo, dndy);

			float mu = eta * Dot(w, n) - Dot(wi, n);
			float dmudx = (eta - (eta * eta * Dot(w, n)) / Dot(wi, n)) * dDNdx;
			float dmudy = (eta - (eta * eta * Dot(w, n)) / Dot(wi, n)) * dDNdy;

			r.rxDirection = wi + eta * dwodx - Vector(mu * dndx + dmudx * n);
			r.ryDirection = wi + eta * dwody - Vector(mu * dndy + dmudy * n);
		}
		RGB Li = renderer->Li(scene, r, sample, rand, arena);
		L = f * Li * AbsDot(wi, n) / pdf;
	}
	return L;
}
