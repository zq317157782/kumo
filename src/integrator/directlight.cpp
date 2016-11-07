/*
 * directlight.cpp
 *
 *  Created on: 2016年8月2日
 *      Author: zhuqian
 */
#include <rgb.h>
#include <scene.h>
#include "directlight.h"
#include "reflection.h"
#include "sampler.h"

RGB DirectLightingIntegrator::Li(const Scene *scene, const Renderer *renderer,
		const RayDifferential &ray, const Sample *sample,
		const Intersection &isect, Random &rnd, MemoryArena& arena) const {
	RGB L(0);
	//获取交点处的bsdf
	BSDF *bsdf = isect.GetBSDF(ray, arena);
	Vector wo = -ray.d;
	const Point &p = bsdf->dgShading.p;
	const Normal &n = bsdf->dgShading.nn;
	//计算交点处的自发光
	L += isect.Le(wo);
	//计算光源对交点的影响
	if (scene->getLightNum() > 0) {
		// Apply direct lighting strategy
		switch (mStrategy) {
		case UNIFORM_ALL:
			L += UniformSampleAllLights(scene, renderer, arena, p, n, wo,
					isect.rayEpsilon, bsdf, sample, rnd,
					mLightSampleOffsets, mBsdfSampleOffsets);
			///cout<<"[Debug]"<<L.r<<L.g<<L.b<<endl;
			break;
		case UNIFORM_ONE:
			L += UniformSampleOneLight(scene, renderer, arena, p, n, wo,
					isect.rayEpsilon, bsdf, sample, rnd,
					mLightNumOffset, mLightSampleOffsets, mBsdfSampleOffsets);
			break;
		}
	}
	if(ray.depth+1<mMaxDepth){
		//cout<<"ray:depth "<<ray.depth<<endl;
		L+=SpecularReflect(ray,bsdf,rnd,isect,renderer,scene,sample,arena);
		//L+=SpecularTransmit(ray,bsdf,rnd,isect,renderer,scene,sample,arena);
	}
	return L;
}

void DirectLightingIntegrator::RequestSamples(Sampler *sampler, Sample *sample,
		const Scene *scene) {
	if (mStrategy == UNIFORM_ALL) {
		int ln = scene->getLightNum();
		mLightSampleOffsets = new LightSampleOffsets[ln];
		mBsdfSampleOffsets = new BSDFSampleOffsets[ln];
		for (int i = 0; i < ln; ++i) {
			Light* light = scene->getLight(i);
			int numSample = light->numSamples;
			//TODO sampler::RoundSize  if(sampler) numSample=sampler->RoundSize(numSample);
			mLightSampleOffsets[i] = LightSampleOffsets(numSample, sample);
			mBsdfSampleOffsets[i] = BSDFSampleOffsets(numSample, sample);
		}
		mLightNumOffset = -1;
	} else if (mStrategy == UNIFORM_ONE) {
		mLightSampleOffsets = new LightSampleOffsets[1];
		mBsdfSampleOffsets = new BSDFSampleOffsets[1];
		mLightSampleOffsets[0] = LightSampleOffsets(1, sample);
		mBsdfSampleOffsets[0] = BSDFSampleOffsets(1, sample);
		mLightNumOffset = sample->Add1D(1);//被选中光源位置的样本偏移
	}
}
