//
// Created by 诸谦 on 16/7/1.
//

#ifndef RAYTRACER_INTEGRATOR_H
#define RAYTRACER_INTEGRATOR_H
#include "global.h"
#include "reflection.h"
//积分器
class Integrator {
public:
	virtual ~Integrator() {
	}
	virtual void RequestSamples(Sampler *sampler, Sample *sample,
			const Scene *scene) {
		//assert(false);
	}
	//
	virtual void Preprocess(const Scene *scene, const Camera *camera,
	                            const Renderer *renderer) {
	}
};

class SurfaceIntegrator: public Integrator {
public:
	virtual RGB Li(const Scene *scene, const Renderer *renderer,
			const RayDifferential &ray,const Sample *sample, const Intersection &isect, Random &rnd,
			MemoryArena& arena) const = 0;
	virtual ~SurfaceIntegrator() {
	}
};

//通过light的能量来计算light的CDF
Distribution1D *ComputeLightSamplingCDF(const Scene *scene);

RGB EstimateDirect(const Scene* scene,const Renderer*renderer,
		MemoryArena& arena,const Light* light,const Point& p,const Normal& n,
		const Vector& wo,float rayEpsilon,const BSDF *bsdf,Random& rand,const LightSample& lightSample,
		const BSDFSample& bsdfSample,BxDFType flags);

//均匀采样所有光源
RGB UniformSampleAllLights(const Scene *scene,
        const Renderer *renderer, MemoryArena &arena, const Point &p,
        const Normal &n, const Vector &wo, float rayEpsilon,BSDF *bsdf, const Sample *sample, Random &rng,
        const LightSampleOffsets *lightSampleOffsets,
        const BSDFSampleOffsets *bsdfSampleOffsets);

RGB UniformSampleOneLight(const Scene *scene,
        const Renderer *renderer, MemoryArena &arena, const Point &p,
        const Normal &n, const Vector &wo, float rayEpsilon,
        BSDF *bsdf, const Sample *sample, Random &rng, int lightNumOffset=-1,
        const LightSampleOffsets *lightSampleOffset=nullptr,
        const BSDFSampleOffsets *bsdfSampleOffset=nullptr);

//计算镜面反射的radiance的工具函数
RGB SpecularReflect(const RayDifferential &ray, BSDF *bsdf,
        Random &rng, const Intersection &isect, const Renderer *renderer,
        const Scene *scene, const Sample *sample, MemoryArena &arena);

////计算镜面折射的radiance的工具函数
RGB SpecularTransmit(const RayDifferential &ray, BSDF *bsdf,
        Random &rng, const Intersection &isect, const Renderer *renderer,
        const Scene *scene, const Sample *sample, MemoryArena &arena);
#endif //RAYTRACER_INTEGRATOR_H
