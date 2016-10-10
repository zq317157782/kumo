/*
 * metropolis.cpp
 *
 *  Created on: 2016年10月10日
 *      Author: zhuqian
 */
#include "metropolis.h"
#include "montecarlo.h"
#include "light.h"
#include "reflection.h"
#include "Intersection.h"
#include "RGB.h"
#include "Scene.h"
//代表路径上的一个vertex的样本
//包括bsdf样本 和 俄罗斯罗盘样本
struct PathSample {
	BSDFSample bsdfSample; //BSDF样本
	float rrSample; //俄罗斯罗盘样本
};

//计算直接光照的时候需要的MIS样本
struct LightingSample {
	BSDFSample bsdfSample; //BSDF样本
	float lightNum; //光源NUM样本
	LightSample lightSample; //光源样本
};

//MLT样本
struct MLTSample {
	CameraSample cameraSample; //相机样本 lens
	vector<PathSample> cameraPathSamples; //相机路径下的Path样本
	vector<LightingSample> lightingSamples; //用于相机路径下 计算直接光照的MIS样本

//双向路径用参数
	float lightNumSample; //用于选择光源NUM
	float lightRaySamples[5]; //用于生成从光源射出的射线的样本
	vector<PathSample> lightPathSamples; //LightPaht下的Path样本

	MLTSample(int maxLength) {
		cameraPathSamples.resize(maxLength);
		lightPathSamples.resize(maxLength);
		lightingSamples.resize(maxLength);
	}
};

//突变模式1 :LargeStep 大突变，为了避免采样局限在整个状态空间的一小部分，同样的，没有实现对时间的采样
static void LargeStep(Random &rng, MLTSample* sample, int maxDepth, float x,
		float y, bool bidirectional) {
	//突变相机sample
	//image panel
	sample->cameraSample.imageX = x;
	sample->cameraSample.imageY = y;
	//lens
	sample->cameraSample.lensU = rng.RandomFloat();
	sample->cameraSample.lensV = rng.RandomFloat();
	//为cameraSampe重新生成随机样本
	for (int i = 0; i < maxDepth; ++i) {
		//path sample
		PathSample &cps = sample->cameraPathSamples[i];
		cps.bsdfSample.uComponent = rng.RandomFloat();
		cps.bsdfSample.uDir[0] = rng.RandomFloat();
		cps.bsdfSample.uDir[1] = rng.RandomFloat();
		cps.rrSample = rng.RandomFloat();
		//直接光样本
		LightingSample &ls = sample->lightingSamples[i];
		ls.bsdfSample.uComponent = rng.RandomFloat();
		ls.bsdfSample.uDir[0] = rng.RandomFloat();
		ls.bsdfSample.uDir[1] = rng.RandomFloat();
		ls.lightNum = rng.RandomFloat();
		ls.lightSample.uComponent = rng.RandomFloat();
		ls.lightSample.uPos[0] = rng.RandomFloat();
		ls.lightSample.uPos[1] = rng.RandomFloat();
	}
	//如果使用了双向路径追踪,还需要整ligthPath的样本
	if (bidirectional) {
		sample->lightNumSample = rng.RandomFloat();
		for (int i = 0; i < 5; ++i) {
			sample->lightRaySamples[i] = rng.RandomFloat();
		}
		for (int i = 0; i < maxDepth; ++i) {
			PathSample &lps = sample->lightPathSamples[i];
			lps.bsdfSample.uComponent = rng.RandomFloat();
			lps.bsdfSample.uDir[0] = rng.RandomFloat();
			lps.bsdfSample.uDir[1] = rng.RandomFloat();
			lps.rrSample = rng.RandomFloat();
		}
	}
}

//按照指数分布 生成一个样本
static inline void mutate(Random& rng, float *v, float min = 0.0f, float max =
		1.0f) {
	if (min == max) {
		*v = min;
		return;
	}
	//PBRT默认的突变范围 至于为啥是这个之间估计是实验得出的比较好的范围
	float a = 1.0f / 1024.0f, b = 1.0f / 64.0f;
	static const float logRatio = -logf(b / a);
	//为什么要乘以(max-min) 我还没有理解
	float delta = (max - min) * b * expf(logRatio * rng.RandomFloat());

	if (rng.RandomFloat() < 0.5f) {
		*v += delta;
		if (*v >= max)
			*v = min + (*v - max);	//wrap out
	} else {
		*v -= delta;
		if (*v < min)
			*v = max - (min - *v);	//wrap out
	}
	//最后的保护措施
	if (*v < min || *v >= max)
		*v = min;
}

//微小的突变的策略
static void SmallStep(Random& rng, MLTSample *sample, int maxDepth, int x0,
		int x1, int y0, int y1, bool bidirectional) {
	//突变cameraSample
	mutate(rng, &sample->cameraSample.imageX, x0, x1);
	mutate(rng, &sample->cameraSample.imageX, y0, y1);
	mutate(rng, &sample->cameraSample.lensU);
	mutate(rng, &sample->cameraSample.lensV);
	//突变path sample
	for (int i = 0; i < maxDepth; ++i) {
		PathSample &cps = sample->cameraPathSamples[i];
		mutate(rng, &cps.bsdfSample.uComponent);
		mutate(rng, &cps.bsdfSample.uDir[0]);
		mutate(rng, &cps.bsdfSample.uDir[1]);
		mutate(rng, &cps.rrSample);

		//直接光照样本
		LightingSample &ls = sample->lightingSamples[i];
		mutate(rng, &ls.bsdfSample.uComponent);
		mutate(rng, &ls.bsdfSample.uDir[0]);
		mutate(rng, &ls.bsdfSample.uDir[1]);
		mutate(rng, &ls.lightNum);
		mutate(rng, &ls.lightSample.uComponent);
		mutate(rng, &ls.lightSample.uPos[0]);
		mutate(rng, &ls.lightSample.uPos[1]);
	}

	//突变light path的样本
	if (bidirectional) {
		mutate(rng, &sample->lightNumSample);
		for (int i = 0; i < 5; ++i) {
			mutate(rng, &sample->lightRaySamples[i]);
		}
		for (int i = 0; i < maxDepth; ++i) {
			PathSample& lps = sample->lightPathSamples[i];
			mutate(rng, &lps.bsdfSample.uComponent);
			mutate(rng, &lps.bsdfSample.uDir[0]);
			mutate(rng, &lps.bsdfSample.uDir[1]);
			mutate(rng, &lps.rrSample);
		}
	}
}

struct PathVertex {
	Intersection isect;	//交点信息
	Vector wPrev, wNext;	//两条射线
	BSDF *bsdf;
	bool specularBounce;	//bsdf是否包含specular组件
	int nSpecularComponents;	//包含多少的specular组件
	RGB alpha;	//throughout
};
static unsigned int GeneratePath(const RayDifferential& r, const RGB& palpha,
		const Scene* scene, MemoryArena& arena,
		const vector<PathSample>& samples, PathVertex* path,
		RayDifferential* escapedRay, RGB* escapedAlpha) {
	RayDifferential ray = r;	//射线
	RGB alpha = palpha;	//初始贡献
	if (escapedAlpha)
		*escapedAlpha = RGB(0.0f);
	unsigned int length = 0;	//当前路径的长度

	for (; length < samples.size(); ++length) {
		//遍历路径样本点 生成路径点

		PathVertex &v = path[length];	//当前处理的路径点
		if (!scene->Intersect(ray, &v.isect)) {
			//没有与任何几何相交的情况
			if (escapedRay)
				*escapedRay = ray;
			if (escapedAlpha)
				*escapedAlpha = alpha;
		}

		v.alpha = alpha;	//更新当前节点的throughout
		BSDF* bsdf = v.isect.GetBSDF(ray, arena);	//获得当前节点的bsdf
		v.bsdf = bsdf;
		v.wPrev = -ray.d;	//初始化顶点的前射线

		float pdf;
		BxDFType flags;
		//采样BSDF
		RGB f = bsdf->Sample_f(-ray.d, &v.wNext, samples[length].bsdfSample,
				&pdf, BSDF_ALL, &flags);
		v.specularBounce = (flags & BSDF_SPECULAR) != 0;
		v.nSpecularComponents = bsdf->NumComponents(BxDFType(BSDF_SPECULAR|BSDF_REFLECTION|BSDF_TRANSMISSION));

		if (f.IsBlack() || pdf == 0.0f) {
			//bsdf不提供任何贡献的情况
			return length + 1;
		}
		const Point &p = bsdf->dgShading.p;	//交点
		const Normal &n = bsdf->dgShading.nn;	//法线
		//计算新的throughout
		RGB pathScale = f * AbsDot(v.wNext, n) / pdf;
		float rrSurviveProb = min(1.0f, pathScale.y());
		//满足罗盘条件 终结
		if (samples[length].rrSample > rrSurviveProb) {
			return length + 1;
		}
		//更新throughout 并且补偿俄罗斯罗盘权重
		alpha *= pathScale / rrSurviveProb;
		//生成新的射线
		ray = RayDifferential(p, v.wNext, ray, v.isect.rayEpsilon);
	}
	return length;
}
