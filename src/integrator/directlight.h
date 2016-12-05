/*
 * directlight.h
 *
 *  Created on: 2016年8月2日
 *      Author: zhuqian
 */

#ifndef INTEGRATOR_DIRECTLIGHT_H_
#define INTEGRATOR_DIRECTLIGHT_H_
#include "kumo.h"
#include "integrator.h"
//采样光的策略
enum LightStrategy{UNIFORM_ALL,UNIFORM_ONE};

//直接光照积分器 只对直接光进行采样，不考虑间接光的影响
class DirectLightingIntegrator:public SurfaceIntegrator{
private:
	LightStrategy mStrategy;
	int mMaxDepth;
	//采样相关的结构
	LightSampleOffsets *mLightSampleOffsets;
	BSDFSampleOffsets *mBsdfSampleOffsets;
	int mLightNumOffset;	//单光源情况下使用的变量
public:

	DirectLightingIntegrator(LightStrategy strategy=UNIFORM_ALL,int maxDepth=2){
		mStrategy=strategy;
		mMaxDepth=maxDepth;
		mLightSampleOffsets=nullptr;
		mBsdfSampleOffsets=nullptr;
		mLightNumOffset=-1;
	}

	~DirectLightingIntegrator(){}

	virtual RGB Li(const Scene *scene, const Renderer *renderer,
				const RayDifferential &ray,const Sample *sample, const Intersection &isect, Random &rnd,
				MemoryArena& arena) const override;

	//根据不同的采样光的策略，申请不同的样本点
	virtual void RequestSamples(Sampler *sampler, Sample *sample,
				const Scene *scene) override;
};


#endif /* INTEGRATOR_DIRECTLIGHT_H_ */
