/*
 * ic.h
 *
 *  Created on: 2016年9月5日
 *      Author: zhuqian
 */

#ifndef SRC_INTEGRATOR_IC_H_
#define SRC_INTEGRATOR_IC_H_
#include "kumo.h"
#include "integrator.h"
#include "octree.h"
#include "parallel.h"

struct IrradianceSample;

//Irradiance Cache
class IrradianceCacheIntegrator: public SurfaceIntegrator {
private:
	mutable RWMutex *mMutex; //读写锁
	LightSampleOffsets *mLightSampleOffsets;
	BSDFSampleOffsets *mBsdfSampleOffsets;
	mutable Octree<IrradianceSample *> *mOctree;
	Float mMinWeight; //
	int mMaxSpecularDepth; //最大镜面反射深度
	Float mCosMaxSampleAngleDifference;
	Float mMaxIndirectDepth; //最大间接反射深度
	unsigned int mNumSample;
	Float minSamplePixelSpacing;
	Float maxSamplePixelSpacing;

	//irradiance 插值
	bool interpolateE(const Scene *scene, const Point &p, const Normal &n,
			RGB *E, Vector *wi) const;
	RGB pathL(Ray &r, const Scene *scene, const Renderer *renderer, Random &rng,
			MemoryArena &arena) const;
	RGB indirectLo(const Point &p, const Normal &ng, Float pixelSpacing,
			const Vector &wo, Float rayEpsilon, BSDF *bsdf, BxDFType flags,
			Random &rng, const Scene *scene, const Renderer *renderer,
			MemoryArena &arena) const;
public:
	IrradianceCacheIntegrator(Float mw, Float minsp, Float maxsp, Float maxang,
			int msd, int maxind, unsigned int ns) {
		mMinWeight = mw;
		mMaxSpecularDepth = msd;
		mCosMaxSampleAngleDifference = cosf(Degrees(maxang));
		mMaxIndirectDepth = maxind;
		mNumSample = ns;
		minSamplePixelSpacing = minsp;
		maxSamplePixelSpacing = maxsp;
		mMutex=new RWMutex();
	}

	virtual void RequestSamples(Sampler *sampler, Sample *sample,
			const Scene *scene) override;
	virtual void Preprocess(const Scene *scene, const Camera *camera,
			const Renderer *renderer) override;
	virtual RGB Li(const Scene *scene, const Renderer *renderer,
			const RayDifferential &ray, const Sample *sample,
			const Intersection &isect, Random &rnd, MemoryArena& arena) const
					override;
};

#endif /* SRC_INTEGRATOR_IC_H_ */
