/*
 * igi.h
 *
 *  Created on: 2016年8月12日
 *      Author: zhuqian
 */

#ifndef SRC_INTEGRATOR_IGI_H_
#define SRC_INTEGRATOR_IGI_H_
#include "global.h"
#include "integrator.h"

// Instant Global Irradiacne

//偽光です　私の日本語がすごいね。
struct VirtualLight {
	VirtualLight() {
	}
	VirtualLight(const Point &pp, const Normal &nn, const RGB &c, float reps) :
			p(pp), n(nn), pathContrib(c), rayEpsilon(reps) {
	}
	Point p;
	Normal n;
	RGB pathContrib;
	float rayEpsilon;
};

class IGIIntegrator: public SurfaceIntegrator {
private:
	LightSampleOffsets *mLightSampleOffsets; //光源样本偏移 用来获取光源样本
	BSDFSampleOffsets *mBsdfSampleOffsets; //BSDF样本偏移
	unsigned int mNumLightPaths, mNumLightSets; //光源路径数 和 光源集合数
	float mGeoLimit; //最小的集合级数
	vector<vector<VirtualLight> > mVirtualLights; //虚拟光源集合
	float mRrThreshold; //俄罗斯罗盘的阈值

	//以下参数还没搞清楚
	int mNumGatherSamples;
	int maxSpecularDepth;
	int mVirtualLightSetOffset;
	BSDFSampleOffsets gatherSampleOffset;
public:
	IGIIntegrator(unsigned int nl, unsigned int ns, float rrt, int maxd,
			float geoLimit, int numGather) {
		mNumLightPaths = RoundUpPow2(nl);
		mNumLightSets = RoundUpPow2(ns);
		mRrThreshold = rrt;
		mVirtualLights.resize(mNumLightSets);
		mGeoLimit = geoLimit;
		mLightSampleOffsets = nullptr;
		mBsdfSampleOffsets = nullptr;
		mVirtualLightSetOffset=0;
		//-----------
		maxSpecularDepth = maxd;
		mNumGatherSamples = numGather;

	}

	virtual void RequestSamples(Sampler *sampler, Sample *sample,
			const Scene *scene) override;
	virtual void Preprocess(const Scene *scene, const Camera *camera,
		                            const Renderer *renderer) override;
	virtual RGB Li(const Scene *scene, const Renderer *renderer,
						const RayDifferential &ray,const Sample *sample, const Intersection &isect, Random &rnd,
						MemoryArena& arena) const override;
};

#endif /* SRC_INTEGRATOR_IGI_H_ */
