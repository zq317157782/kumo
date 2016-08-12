/*
 * path.h
 *
 *  Created on: 2016年8月5日
 *      Author: zhuqian
 */

#ifndef INTEGRATOR_PATH_H_
#define INTEGRATOR_PATH_H_
#include "global.h"
#include "integrator.h"
#include "light.h"

//路径追踪器
class PathIntegrator: public SurfaceIntegrator {
private:
	int mMaxDepth; //最大反射深度
#define SAMPLE_DEPTH 3 //用于设计好的采样的深度
	LightSampleOffsets lightSampleOffsets[SAMPLE_DEPTH]; //光源的样本 用于MIS
	int lightNumOffset[SAMPLE_DEPTH];
	BSDFSampleOffsets bsdfSampleOffsets[SAMPLE_DEPTH]; //bsdf的样本 用于MIS
	BSDFSampleOffsets pathSampleOffsets[SAMPLE_DEPTH]; //用于采样路径
public:
	PathIntegrator(int maxDepth){
		mMaxDepth=maxDepth;//最大的深度值
	}
	~PathIntegrator(){} //析构函数

	virtual void RequestSamples(Sampler *sampler, Sample *sample,
					const Scene *scene) override;

	virtual RGB Li(const Scene *scene, const Renderer *renderer,
					const RayDifferential &ray,const Sample *sample, const Intersection &isect, Random &rnd,
					MemoryArena& arena) const override;

};

#endif /* INTEGRATOR_PATH_H_ */
