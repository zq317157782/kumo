/*
 * integrator.cpp
 *
 *  Created on: 2016年7月30日
 *      Author: zhuqian
 */
#include "integrator.h"
#include "global.h"
#include "light.h"
#include "Scene.h"
#include "montecarlo.h"
Distribution1D *ComputeLightSamplingCDF(const Scene *scene) {
	unsigned int nLights = scene->getLightNum();
	vector<float> lightPower(nLights, 0.0f);
	for (unsigned int i = 0; i < nLights; ++i)
		lightPower[i] = scene->getLight(i)->Power(scene).y();
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
			Ld+=EstimateDirect(scene,renderer,arena,light,p,n,wo,rayEpsilon,bsdf,rand,lightSample,bsdfSample,BxDFType(BSDF_ALL&~BSDF_SPECULAR));
		}
		L+=Ld/numSample;
	}
	return L;
}

RGB UniformSampleOneLight(const Scene *scene,
        const Renderer *renderer, MemoryArena &arena, const Point &p,
        const Normal &n, const Vector &wo, float rayEpsilon,
        BSDF *bsdf, const Sample *sample, Random &rng, int lightNumOffset,
        const LightSampleOffsets *lightSampleOffset,
        const BSDFSampleOffsets *bsdfSampleOffset){
//TODO 实现UniformSampleOneLight
}
