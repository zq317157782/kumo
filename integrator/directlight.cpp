/*
 * directlight.cpp
 *
 *  Created on: 2016年8月2日
 *      Author: zhuqian
 */
#include "directlight.h"
#include "RGB.h"
#include "scene.h"
#include "reflection.h"
#include "sampler.h"

RGB DirectLightingIntegrator::Li(const Scene *scene, const Renderer *renderer,
		const RayDifferential &ray, const Sample *sample,
		const Intersection &isect, Random &rnd, MemoryArena& arena) const {

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
