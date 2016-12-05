/*
 * path.cpp
 *
 *  Created on: 2016年8月5日
 *      Author: zhuqian
 */

#include <rgb.h>
#include <scene.h>
#include "path.h"
#include "reflection.h"
#include "sampler.h"

void PathIntegrator::RequestSamples(Sampler *sampler, Sample *sample,
		const Scene *scene) {
	//申请相应的样本点
	for (int i = 0; i < SAMPLE_DEPTH; ++i) {
		lightSampleOffsets[i] = LightSampleOffsets(1, sample);
		lightNumOffset[i] = sample->Add1D(1);
		bsdfSampleOffsets[i] = BSDFSampleOffsets(1, sample);
		pathSampleOffsets[i] = BSDFSampleOffsets(1, sample);
	}
}

RGB PathIntegrator::Li(const Scene *scene, const Renderer *renderer,
		const RayDifferential &r, const Sample *sample,
		const Intersection &isect, Random &rnd, MemoryArena& arena) const {
	RGB pathThroughput = RGB(1); //sum(f*cos/p)
	RGB L = RGB(0);
	RayDifferential ray(r);
	bool specularBounce = false; //判断最后一次反射是不是镜面反射
	Intersection localIsect;
	const Intersection *isectp = &isect; //这里初始化为相机射出的射线的第一个交点

	//这个循环就是整个路径追踪算法的过程 bounces代表已经反射了几次
	for (int bounces = 0;; ++bounces) {
		if (bounces == 0 || specularBounce) {
			L += pathThroughput * isectp->Le(-ray.d); //如果是第一个交点 或者上次是镜面反射,添加自发能量
		}

		BSDF *bsdf = isectp->GetBSDF(ray, arena); //获取交点处的bsdf
		const Point &p = bsdf->dgShading.p;
		const Normal &n = bsdf->dgShading.nn;
		Vector3f wo = -ray.d; //出射方向
		if (bounces < SAMPLE_DEPTH) { //设计好的样本
			L += pathThroughput
					* UniformSampleOneLight(scene, renderer, arena, p, n, wo,
							isectp->rayEpsilon, bsdf, sample, rnd,
							lightNumOffset[bounces],
							&lightSampleOffsets[bounces],
							&bsdfSampleOffsets[bounces]); //计算当前路径的能量
		} else { //随机样本
			L += pathThroughput
					* UniformSampleOneLight(scene, renderer, arena, p, n, wo,
							isectp->rayEpsilon, bsdf, sample, rnd);
		}

		//准备采样出射方向
		BSDFSample outgoingBSDFSample;
		if (bounces < SAMPLE_DEPTH)
			outgoingBSDFSample = BSDFSample(sample, pathSampleOffsets[bounces],
					0);
		else
			outgoingBSDFSample = BSDFSample(rnd);

		Vector3f wi;
		Float pdf;
		BxDFType flags;
		RGB f = bsdf->Sample_f(wo, &wi, outgoingBSDFSample, &pdf, BSDF_ALL,
				&flags); //根据bsdf采样出射方向
		if (f.IsBlack() || pdf == 0.0f)
			break;
		specularBounce = (flags & BSDF_SPECULAR) != 0; //判断bsdf是否是镜面
		pathThroughput *= f * AbsDot(wi, n) / pdf; //更新 pathThroughput
		ray = RayDifferential(p, wi, ray, isectp->rayEpsilon); //生成新的射线
		//反射数大于3 就开始使用俄罗斯罗盘
		if (bounces > 3) {
			Float continueProbability = std::min(0.5f, pathThroughput.luminance());
			if (rnd.RandomFloat() > continueProbability)
				break;
			pathThroughput =pathThroughput/continueProbability;
		}
		//反射达到最大值 就不需要寻找下个点了
		if (bounces == mMaxDepth)
			break;

		if (!scene->Intersect(ray, &localIsect)) {
			if (specularBounce) {//如果最后一次反射是镜面反射，需要把没形体的光源的能量加入
				for (uint32_t i = 0; i < scene->getLightNum(); ++i)
					L += pathThroughput * scene->getLight(i)->Le(ray);
			}
			break;
		}
//TODO  这里应该还有透射的代码
		isectp = &localIsect;//更新交点
	}
	return L;
}

