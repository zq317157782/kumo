//
// Created by 诸谦 on 16/6/25.
//

#include "Camera.h"
#include "simpleRenderer.h"
#include "primitive.h"
#include "integrator.h"
#include "sampler.h"
#include "random.h"
#include "film.h"
#include "Scene.h"

//void SimpleRenderer::render(const Scene* scene) {
//	Sample sample(sampler, mSurfaceIntegrator, scene);
//	Sample* samples=sample.Duplicate(sampler->samplesPerPixel);
//	int nSample = 0;
//	while ((nSample = sampler->GetMoreSamples(samples, mRand)) > 0) {
//		for (int i = 0; i < nSample; ++i) {
//			RGB L;
//			Point point;
//			point.x = samples[i].imageX - camera->film->xResolution * 0.5f;
//			point.y = samples[i].imageY - camera->film->yResolution * 0.5f;
//			Ray ray = camera->generateRay(point);
//			Intersection sr;
//			L = Li(scene, ray, &samples[i], mRand, mArena, &sr);
//			camera->film->AddSample(samples[i], L);
//		}
//		mArena.FreeAll();
//	}
//
//	camera->film->WriteImage(1.0f);
//}

void SimpleRenderer::render(const Scene* scene) {
	Sample sample(sampler, mSurfaceIntegrator, scene);
	int nPixels = camera->film->xResolution * camera->film->yResolution;
	int nTasks = max(32 * CORE_NUM, nPixels / (16 * 16));
	nTasks=RoundUpPow2(nTasks);
	vector<Task *> renderTasks;
	for (int i = 0; i < nTasks; ++i) {
		renderTasks.push_back(
				new SimpleRendererTask(scene, this, camera, sampler, &sample,
						nTasks - 1 - i, nTasks));
	}
	EnqueueTasks(renderTasks);
	WaitForAllTasks();
	for (uint32_t i = 0; i < renderTasks.size(); ++i)
		delete renderTasks[i];
	camera->film->WriteImage(1.0f);
}

RGB SimpleRenderer::Li(const Scene *scene, const RayDifferential &ray,
		const Sample *sample, Random &rng, MemoryArena &arena,
		Intersection *isect, RGB *T) const {
	Intersection localIsect;
	if (!isect)
		isect = &localIsect;
	RGB Li = 0.f;
	if (scene->Intersect(ray, isect)) {
		Li = mSurfaceIntegrator->Li(scene, this, ray, sample, *isect, rng,
				arena);
	} else { //处理没有形体的光源能量
		for (unsigned int i = 0; i < scene->getLightNum(); ++i)
			Li += scene->getLight(i)->Le(ray);
		// Li+=scene->background;
	}
	return Li;
}

SimpleRendererTask::SimpleRendererTask(const Scene *scene, Renderer* renderer,
		Camera* c, Sampler* s, Sample *sample, int count, int tasks) {
	mCount = count;
	mTaskNum = tasks;
	mSampler = s->GetSubSampler(mCount, mTaskNum);
	mCamerea = c;
	mSample = sample;
	mScene = scene;
	mRenderer = renderer;
	mRand = Random(count);
}

void SimpleRendererTask::Run() {
	cout << "task:" << mCount << " max:" << mTaskNum <<" thread id "<<this_thread::get_id()<<endl;
	//cout<<mSampler<<endl;
	Sample* samples = mSample->Duplicate(mSampler->samplesPerPixel);
	int nSample = 0;
	while ((nSample = mSampler->GetMoreSamples(samples, mRand)) > 0) {
		for (int i = 0; i < nSample; ++i) {
			RGB L;
			Point point;
			point.x = samples[i].imageX - mCamerea->film->xResolution * 0.5f;
			point.y = samples[i].imageY - mCamerea->film->yResolution * 0.5f;
			Ray ray = mCamerea->generateRay(point);
			Intersection sr;
			L = mRenderer->Li(mScene, ray, &samples[i], mRand, mArena, &sr);
			mCamerea->film->AddSample(samples[i], L);
		}
		mArena.FreeAll();
	}
}
