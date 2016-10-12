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
#include "Camera.h"
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
		v.nSpecularComponents = bsdf->NumComponents(
				BxDFType(BSDF_SPECULAR | BSDF_REFLECTION | BSDF_TRANSMISSION));

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

//PT下计算radiance
RGB MetropolisRenderer::LPath(const Scene *scene, const PathVertex *path,
		int pathLength, MemoryArena &arena,
		const vector<LightingSample> &samples, Random &rng,
		const Distribution1D *lightDistribution,
		const RayDifferential &escapedRay, const RGB &escapedAlpha) const {

	RGB L = 0.0f;		//最终返回的radiance值
	bool previousSpecular = true;		//判断前一次bsdf是否是界面反射或者折射 从相机到第一个顶点默认算界面反射
	bool allSpecular = true; //到当前path顶点为止是否都为镜面BSDF
	for (int i = 0; i < pathLength; ++i) {
		//遍历处理每一个Path顶点
		const PathVertex &vc = path[i];
		const Point &pc = vc.bsdf->dgShading.p; //当前相交点
		const Normal &nc = vc.bsdf->dgShading.nn; //当前法线

		//这里也包含了直接从相机射出的射线所相交的交点
		if (previousSpecular && (mDirectLighting == nullptr || !allSpecular)) {
			//1.MLT Path下 如果是镜面BSDF，需要考虑自发光，因为EstimeateDirect在Specular下并没有考虑自发光
			//2.DirectLighting下 前面连续的镜面自发光已经被DirectLighting考虑，需要从非镜面BSDF后面的镜面BSDF开始考虑自发光
			L += vc.alpha * vc.isect.Le(vc.wPrev);
		}

		//开始计算直接光
		RGB Ld = 0.0f;
		//满足DirectLighting 没有处理的情况
		if (mDirectLighting == nullptr && !allSpecular) {
			const LightingSample &ls = samples[i];
			float lightPdf;
			int lightNum = lightDistribution->SampleDiscrete(ls.lightNum,
					&lightPdf);
			const Light* light = scene->getLight(lightNum);			//获得采样后的光源
			//计算直接光(不包括Specular BSDF)
			Ld = vc.alpha
					* EstimateDirect(scene, this, arena, light, pc, nc,
							vc.wPrev, vc.isect.rayEpsilon, vc.bsdf, rng,
							ls.lightSample, ls.bsdfSample,
							BxDFType(BSDF_ALL & ~BSDF_SPECULAR)) / lightPdf;
		}

		//更新数据
		previousSpecular = vc.specularBounce;
		allSpecular &= previousSpecular;
		L += Ld;

	}
	//如果最终路径没有射到任何物体，需要考虑infinate Lights
	if (!escapedAlpha.IsBlack() && previousSpecular
			&& (mDirectLighting == nullptr || !allSpecular)) {
		for (uint32_t i = 0; i < scene->getLightNum(); ++i)
			L += escapedAlpha * scene->getLight(i)->Le(escapedRay);
	}
	return L;
}

//使用BPT计算radiance
RGB MetropolisRenderer::LBidir(const Scene* scene, const PathVertex* cameraPath,
		int cameraPathLength, const PathVertex* lightPath, int lightPathLength,
		MemoryArena &arena, const vector<LightingSample>& samples, Random& rng,
		const Distribution1D* lightDistribution,
		const RayDifferential &escapedRay, const RGB &escapedAlpha) const {
	RGB L = 0.0f;		//最终返回的radiance值
	bool previousSpecular = true;		//判断前一次bsdf是否是界面反射或者折射 从相机到第一个顶点默认算界面反射
	bool allSpecular = true; //到当前path顶点为止是否都为镜面BSDF

	//以下区块 计算路径中specular的情况
	int nVerts = cameraPathLength + lightPathLength + 2; //最大的路径的顶点个数
	int *nSpecularVertices = ALLOCA(int, nVerts); //分配一个储存各个长度下路径包含specular组件的数组
	memset(nSpecularVertices, 0, nVerts * sizeof(int)); //全部初始化为0
	for (int i = 0; i < cameraPathLength; ++i) {
		for (int j = 0; j < lightPathLength; ++j) {
			if (cameraPath[i].specularBounce || lightPath[j].specularBounce) {
				++nSpecularVertices[i + j + 2]; //记录specular出现的情况
			}
		}
	}

	for (int i = 0; i < cameraPathLength; ++i) {
		//遍历处理每一个Path顶点
		const PathVertex &vc = cameraPath[i];
		const Point &pc = vc.bsdf->dgShading.p; //当前相交点
		const Normal &nc = vc.bsdf->dgShading.nn; //当前法线
		//这里也包含了直接从相机射出的射线所相交的交点
		if (previousSpecular && (mDirectLighting == nullptr || !allSpecular)) {
			//1.MLT Path下 如果是镜面BSDF，需要考虑自发光，因为EstimeateDirect在Specular下并没有考虑自发光
			//2.DirectLighting下 前面连续的镜面自发光已经被DirectLighting考虑，需要从非镜面BSDF后面的镜面BSDF开始考虑自发光
			L += vc.alpha * vc.isect.Le(vc.wPrev);
		}

		//开始计算直接光
		RGB Ld = 0.0f;
		//满足DirectLighting 没有处理的情况
		if (mDirectLighting == nullptr && !allSpecular) {
			const LightingSample &ls = samples[i];
			float lightPdf;
			int lightNum = lightDistribution->SampleDiscrete(ls.lightNum,
					&lightPdf);
			const Light* light = scene->getLight(lightNum);			//获得采样后的光源
			//计算直接光(不包括Specular BSDF)
			Ld = vc.alpha
					* EstimateDirect(scene, this, arena, light, pc, nc,
							vc.wPrev, vc.isect.rayEpsilon, vc.bsdf, rng,
							ls.lightSample, ls.bsdfSample,
							BxDFType(BSDF_ALL & ~BSDF_SPECULAR)) / lightPdf;
		}

		//更新数据
		previousSpecular = vc.specularBounce;
		allSpecular &= previousSpecular;
		L += Ld / (i + 1 - nSpecularVertices[i + 1]);//这里需要对Ld根据所有相同长度路径中Specular的个数进行reweight;

		//在当前path顶点不为specular的情况下，可以尝试和lightPath进行连接
		if (!vc.specularBounce) {
			for (int j = 0; j < lightPathLength; ++j) {
				const PathVertex &vl = lightPath[j];
				const Point &pl = vl.bsdf->dgShading.p; //ligthPath下的交点
				const Normal &nl = vl.bsdf->dgShading.nn; //ligthPath下的交点的法线
				//lightPath这边也不是specular,可以尝试连接
				if (!vl.specularBounce) {
					Vector w = Normalize(pl - pc);
					RGB fc = vc.bsdf->f(vc.wPrev, w)
							* (1 + vc.nSpecularComponents);	//1+vc.nSpecularComponents factor用来reweight BSDF的贡献
					RGB fl = vl.bsdf->f(-w, vl.wPrev)
							* (1 + vl.nSpecularComponents);
					if (fc.IsBlack() || fl.IsBlack())
						continue;	//跳过当前ligthPath
					Ray r(pc, pl - pc, 1e-3f, 0.999f);
					//cameraPath和lightPath之间没有阻挡
					if (!scene->IntersectP(r)) {
						float pathWt = 1.0f
								/ (i + j + 2 - nSpecularVertices[i + j + 2]);
						float G = AbsDot(nc, w) * AbsDot(nl, w)
								/ DistanceSqr(pl, pc);
						L += (vc.alpha * fc * G * fl * vl.alpha) * pathWt;//BPT公式
					}
				}
			}
		}
	}

	//如果最终路径没有射到任何物体，需要考虑infinate Lights
	if (!escapedAlpha.IsBlack() && previousSpecular
			&& (mDirectLighting == nullptr || !allSpecular)) {
		for (uint32_t i = 0; i < scene->getLightNum(); ++i)
			L += escapedAlpha * scene->getLight(i)->Le(escapedRay);
	}
	return L;
}

RGB MetropolisRenderer::PathL(const MLTSample &sample, const Scene*scene,
		MemoryArena& arena, const Camera* camera,
		const Distribution1D* lightDistribution, PathVertex *cameraPath,
		PathVertex* lightPath, Random& rng) const {
	RayDifferential cameraRay;
	//生成射线
	float cameraWt = camera->GenerateRayDifferential(sample.cameraSample,
			&cameraRay);
	cameraRay.ScaleDifferentials(1.0f / sqrtf(mNumPixelSamples));

	RayDifferential escapedRay;
	RGB escapedAlpha;
	//生成路径
	unsigned int cameraLength = GeneratePath(cameraRay, cameraWt, scene, arena,
			sample.cameraPathSamples, cameraPath, &escapedRay, &escapedAlpha);

	if (!mBidirectional) {
		return LPath(scene, cameraPath, cameraLength, arena,
				sample.lightingSamples, rng, lightDistribution, escapedRay,
				escapedAlpha);
	} else {
		float lightPdf, lightRayPdf;
		//采样光源
		unsigned int lightNum = lightDistribution->SampleDiscrete(
				sample.lightNumSample, &lightPdf);
		const Light *light = scene->getLight(lightNum);
		Ray lightRay;
		Normal Nl;
		LightSample lrs(sample.lightRaySamples[0], sample.lightRaySamples[1],
				sample.lightRaySamples[2]);
		//采样光源射线
		RGB lightWt = light->Sample_L(scene, lrs, sample.lightRaySamples[3],
				sample.lightRaySamples[4], &lightRay, &Nl, &lightRayPdf);
		if (lightWt.IsBlack() || lightRayPdf == 0.0f) {
			return LPath(scene, cameraPath, cameraLength, arena,
					sample.lightingSamples, rng, lightDistribution, escapedRay,
					escapedAlpha);
		} else {
			//双向路径追踪
			lightWt *= AbsDot(Normalize(Nl), lightRay.d)
					/ (lightPdf * lightRayPdf);
			unsigned int lightLength = GeneratePath(RayDifferential(lightRay),
					lightWt, scene, arena, sample.lightPathSamples, lightPath,
					nullptr, nullptr);

			return LBidir(scene, cameraPath, cameraLength, lightPath,
					lightLength, arena, sample.lightingSamples, rng,lightDistribution, escapedRay,
					escapedAlpha);
		}
	}
}
