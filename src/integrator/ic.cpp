/*
 * ic.cpp
 *
 *  Created on: 2016年9月5日
 *      Author: zhuqian
 */
#include <camera.h>
#include <scene.h>
#include "ic.h"
#include "geometry.h"
#include "sampler.h"
#include "film.h"
#include "parallel.h"
#include "sampler/lowdiscrepancy.h"

//生成irradiance的任务
struct IrradiancePrimeTask: public Task {
	IrradiancePrimeTask(const Scene *sc, const Renderer *sr, const Camera *c,
			Sampler *samp, Sample *s, IrradianceCacheIntegrator *ic, int tn,
			int nt) {
		scene = sc;
		renderer = sr;
		camera = c;
		origSample = s;
		sampler = samp->GetSubSampler(tn, nt);
		irradianceCache = ic;
		taskNum = tn;
		numTasks = nt;
	}
	void Run();

	const Scene *scene;
	const Camera *camera;
	const Renderer *renderer;
	Sampler *sampler;
	Sample *origSample;
	IrradianceCacheIntegrator *irradianceCache;
	int taskNum, numTasks;
};

void IrradiancePrimeTask::Run() {
	if (!sampler) {
		return;
	}
	MemoryArena arena;
	int sampleCount;
	Random rng(29 * taskNum);
	int maxSamples = sampler->MaximumSampleCount();
	Sample *samples = origSample->Duplicate(maxSamples);
	while ((sampleCount = sampler->GetMoreSamples(samples, rng)) > 0) {
		for (int i = 0; i < sampleCount; ++i) {
			RayDifferential ray;
			camera->GenerateRayDifferential(samples[i], &ray);
			Intersection isect;
			if (scene->Intersect(ray, &isect))
				(void) irradianceCache->Li(scene, renderer, ray, &samples[i],
						isect, rng, arena);
		}
		arena.FreeAll();
	}
	delete[] samples;
	delete sampler;
}

//irradiance样本
struct IrradianceSample {
	IrradianceSample() {
	}
	IrradianceSample(const RGB& e, const Point& P, const Normal&N,
			const Vector3f&d, Float md) {
		E = e;
		n = N;
		p = P;
		wAvg = d;
		maxDistance = md;
	}
	RGB E; //irradiance
	Normal n;
	Point p; // world space point 世界坐标系空间点
	Vector3f wAvg; //平均方向
	Float maxDistance; //max valid distance 最大有效距离
};

//用来处理有效irradiance的过程
struct IrradianceProcess {
	IrradianceProcess(const Point&P, const Normal&N, Float minW, Float maxcos) {
		p = P;
		n = N;
		minWeight = minW;
		cosMaxSampleAngleDifference = maxcos;
		numFound = 0;
		sumWeight = 0;
		E = 0;
		wAvg = Vector3f(0, 0, 0);
	}
	//判断当前权重和是否大于最小值
	bool Successful() {
		return sumWeight >= minWeight;
	}

	RGB GetIrradiance() const {
		return E / sumWeight;
	}

	//获取平均方向
	Vector3f GetAverageDirection() const {
		return wAvg;
	}

	bool operator()(const IrradianceSample* sample) {
		//点和样本之间的距离误差
		Float perr = Distance(p, sample->p) / sample->maxDistance;

		Float nerr = sqrtf((1.f - Dot(n, sample->n)) //夹角越小，值越小
		/ (1.f - cosMaxSampleAngleDifference));
		//获取最大error值
		Float err = std::max(perr, nerr);

		//满足valid
		if (err < 1.0f) {
			numFound++;
			Float weight = 1 - err;
			E += weight * sample->E; //累积irradiance
			wAvg += weight * sample->wAvg;
			sumWeight += weight; //累积weight
		}
		return true;
	}

	Point p;
	Normal n;
	Float minWeight;
	Float cosMaxSampleAngleDifference;
	Float sumWeight;
	int numFound;
	RGB E; //irradiance
	Vector3f wAvg; //平均方向

};

void IrradianceCacheIntegrator::RequestSamples(Sampler *sampler, Sample *sample,
		const Scene *scene) {
	//为每个光源准备生成sample的相关数据
	unsigned int numLight = scene->getLightNum();
	mLightSampleOffsets = new LightSampleOffsets[numLight];
	mBsdfSampleOffsets = new BSDFSampleOffsets[numLight];
	for (unsigned int i = 0; i < numLight; ++i) {
		const Light *light = scene->getLight(i);
		int numSamples = light->numSamples; //每个光源需要的样本数
		if (sampler)
			numSamples = sampler->RoundSize(numSamples);
		mLightSampleOffsets[i] = LightSampleOffsets(numSamples, sample);
		mBsdfSampleOffsets[i] = BSDFSampleOffsets(numSamples, sample);
	}
}

void IrradianceCacheIntegrator::Preprocess(const Scene *scene,
		const Camera *camera, const Renderer *renderer) {
	BBox wb = scene->WorldBound();
	Vector3f delta = 0.01f * (wb.pMax - wb.pMin);
	//扩张WorldBound 消除浮点型数据导致的误差
	wb.pMin -= delta;
	wb.pMax += delta;
	//创建存放irradiance sample的octree
	mOctree = new Octree<IrradianceSample*>(wb);
	mMinWeight *= 1.5f;
	int xstart, xend, ystart, yend;
	camera->film->GetSampleExtent(&xstart, &xend, &ystart, &yend);
	//todo PBRT使用的是Halton采样器
	LDSampler sampler(xstart, xend, ystart, yend, 1);
	Sample *sample = new Sample(&sampler, this, scene);

	//这里为什么要运行46个相同的任务 我非常费解
	const int numTasks = 64; //任务数量
	std::vector<Task*> tasks;
	for (int i = 0; i < numTasks; ++i) {
		tasks.push_back(
				new IrradiancePrimeTask(scene, renderer, camera, &sampler,
						sample, this, i, numTasks));
	}
	EnqueueTasks(tasks);
	WaitForAllTasks();
	for (unsigned int i = 0; i < tasks.size(); ++i) {
		delete tasks[i];
	}
	delete sample;
	mMinWeight /= 1.5f;
}

RGB IrradianceCacheIntegrator::Li(const Scene *scene, const Renderer *renderer,
		const RayDifferential &r, const Sample *sample,
		const Intersection &isect, Random &rnd, MemoryArena& arena) const {
	RGB L(0.0);
	BSDF* bsdf = isect.GetBSDF(r, arena); //获得BSDF
	Vector3f wo = -r.d; //出射方向
	const Point &p = bsdf->dgShading.p;
	const Normal &n = bsdf->dgShading.nn;
	L += isect.Le(wo); //添加自发光
	//采样直接光
	L += UniformSampleAllLights(scene, renderer, arena, p, n, wo,
			isect.rayEpsilon, bsdf, sample, rnd, mLightSampleOffsets,
			mBsdfSampleOffsets);
	if (r.depth + 1 < mMaxSpecularDepth) {
		L += SpecularReflect(r, bsdf, rnd, isect, renderer, scene, sample,
				arena);
		L += SpecularTransmit(r, bsdf, rnd, isect, renderer, scene, sample,
				arena);
	}
	Normal ng = isect.dg.nn;
	ng = Faceforward(ng, wo);
	//计算一个像素在世界空间中的长度
	Float pixelSpacing = sqrtf(Cross(isect.dg.dpdx, isect.dg.dpdy).Length());
	BxDFType flags = BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE | BSDF_GLOSSY);
	//反射
	L += indirectLo(p, ng, pixelSpacing, wo, isect.rayEpsilon, bsdf, flags, rnd,
			scene, renderer, arena);
	flags = BxDFType(BSDF_TRANSMISSION | BSDF_DIFFUSE | BSDF_GLOSSY);
	//折射
	L += indirectLo(p, -ng, pixelSpacing, wo, isect.rayEpsilon, bsdf, flags,
			rnd, scene, renderer, arena);
	return L;
}

bool IrradianceCacheIntegrator::interpolateE(const Scene *scene, const Point &p,
		const Normal &n, RGB *E, Vector3f *wi) const {
	//如果octree不存在  直接返回
	if (!mOctree)
		return false;
	IrradianceProcess proc(p, n, mMinWeight, mCosMaxSampleAngleDifference);
	RWMutexLock lock(mMutex);
	//查询并计算平均irradiance
	mOctree->Lookup(p, proc);
	if (!proc.Successful())
		return false;
	*E = proc.GetIrradiance();
	*wi = proc.GetAverageDirection();
	return true;
}

RGB IrradianceCacheIntegrator::pathL(Ray &r, const Scene *scene,
		const Renderer *renderer, Random &rng, MemoryArena &arena) const {
	RGB L(0);
	RGB pathThroughput = 1.0;
	RayDifferential ray(r);
	bool specularBounce = false;
	for (int pathLength = 0;; ++pathLength) {
		Intersection isect;
		if (!scene->Intersect(ray, &isect))
			break;
		if (pathLength == 0)
			r.maxT = ray.maxT;
		//todo 散射代码
		//pathThroughput *= renderer->Transmittance(scene, ray, NULL, rng, arena);
		if (specularBounce)
			L += pathThroughput * isect.Le(-ray.d);
		// Evaluate BSDF at hit point
		BSDF *bsdf = isect.GetBSDF(ray, arena);
		// Sample illumination from lights to find path contribution
		const Point &p = bsdf->dgShading.p;
		const Normal &n = bsdf->dgShading.nn;
		Vector3f wo = -ray.d;
		L += pathThroughput
				* UniformSampleOneLight(scene, renderer, arena, p, n, wo,
						isect.rayEpsilon, bsdf, NULL, rng);
		if (pathLength + 1 == mMaxIndirectDepth)
			break;
		// Sample BSDF to get new path direction
		// Get random numbers for sampling new direction, \mono{bs1}, \mono{bs2}, and \mono{bcs}
		Vector3f wi;
		Float pdf;
		BxDFType flags;
		RGB f = bsdf->Sample_f(wo, &wi, BSDFSample(rng), &pdf, BSDF_ALL,
				&flags);
		if (f.IsBlack() || pdf == 0.)
			break;
		specularBounce = (flags & BSDF_SPECULAR) != 0;
		pathThroughput *= f * AbsDot(wi, n) / pdf;
		ray = RayDifferential(p, wi, ray, isect.rayEpsilon);
		if (pathLength > 2) {
			Float rrProb = std::min(1.f, pathThroughput.luminance());
			if (rng.RandomFloat() > rrProb)
				break;
			pathThroughput = pathThroughput / rrProb;
		}
	}
	return L;
}

//间接光
RGB IrradianceCacheIntegrator::indirectLo(const Point &p, const Normal &ng,
		Float pixelSpacing, const Vector3f &wo, Float rayEpsilon, BSDF *bsdf,
		BxDFType flags, Random &rng, const Scene *scene,
		const Renderer *renderer, MemoryArena &arena) const {
	if (bsdf->NumComponents(flags) == 0)
		return RGB(0.0);
	RGB E;		//irradiance
	Vector3f wi;
	//寻找合适的orradiance sample
	if (!interpolateE(scene, p, ng, &E, &wi)) {
		unsigned int scramble[2] = { rng.RandomUInt(), rng.RandomUInt() };
		Float minHitDistance = INFINITY;		//最近相交参数
		Vector3f wAvg(0, 0, 0);		//平均方向
		RGB LiSum(0);		//入射Li总和
		for (int i = 0; i < mNumSample; ++i) {
			Float u[2];
			Sample02(i, scramble, u);		//产生随机数
			Vector3f w = CosSampleHemisphere(u[0], u[1]);		//生成依赖于cos分布的随机半球向量
			RayDifferential r(p, bsdf->LocalToWorld(w), rayEpsilon);
			r.d = Faceforward(r.d, ng);		//转化向量位置使其和法线在同一半球
			RGB L = pathL(r, scene, renderer, rng, arena);	//计算当前射线上的radiance
			LiSum += L;		//加入到和中
			wAvg += r.d * L.luminance();		//根据能量值 计算平均入射方向
			minHitDistance = std::min(minHitDistance, r.maxT);
		}
		E = (Pi / Float(mNumSample)) * LiSum;		//蒙特卡洛估计
//
//		//计算sample的影响范围
		Float maxDist = maxSamplePixelSpacing * pixelSpacing;
		Float minDist = minSamplePixelSpacing * pixelSpacing;
		Float contribExtent = Clamp(minHitDistance / 2.0f, minDist, maxDist);
		BBox sampleExtent(p);
		sampleExtent.Expand(contribExtent);
//		//生成新的 irradiancesample
		IrradianceSample *sample = new IrradianceSample(E, p, ng, wAvg,
				contribExtent);
		RWMutexLock lock(mMutex);
		lock.upgrade2Writer();
		mOctree->Add(sample, sampleExtent);
		lock.down2Reader();
		wi = wAvg;
	}
	//判断方向的长度是否为0
	if (wi.LengthSqr() == 0.0f)
		return RGB(0);
	//这里是通过一个近似公式算得irradiance
	return bsdf->f(wo, Normalize(wi), flags) * E;
}

