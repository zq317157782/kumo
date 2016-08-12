//
// Created by 诸谦 on 16/6/25.
//

#ifndef RAYTRACER_SIMPLERENDERER_H
#define RAYTRACER_SIMPLERENDERER_H
#include "global.h"
#include "renderer.h"
#include "memory.h"
#include "random.h"
#include "parallel.h"

class SimpleRenderer :public Renderer{
private:
    Camera* camera;
    Sampler* sampler;
    SurfaceIntegrator* mSurfaceIntegrator;
    Random mRand;
    MemoryArena mArena;
public:

    SimpleRenderer(Camera* c,Sampler* s,SurfaceIntegrator* si):Renderer(),camera(c),sampler(s),mSurfaceIntegrator(si){
       // assert(camera!=nullptr);
        //assert(sampler!=nullptr);
    }
    virtual void render(const Scene* scene) override;

    //忽略透射
    virtual RGB Li(const Scene *scene, const RayDifferential &ray,
               const Sample *sample, Random &rng, MemoryArena &arena,
               Intersection *isect = nullptr, RGB *T = nullptr) const override;

};

class SimpleRendererTask:public Task{
private:
	int mCount;
	int mTaskNum;
	Sampler* mSampler;
	Camera* mCamerea;
	Sample *mSample;
	const Scene *mScene;
	Renderer* mRenderer;
	Random mRand;
	MemoryArena mArena;
public:
	SimpleRendererTask(const Scene *scene,Renderer* renderer,Camera* c,Sampler* s,Sample *sample,int count,int tasks);

	virtual void Run() override;
};

#endif //RAYTRACER_SIMPLERENDERER_H
