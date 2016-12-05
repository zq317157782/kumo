/*
 * igi.cpp
 *
 *  Created on: 2016年8月12日
 *      Author: zhuqian
 */
#include <scene.h>
#include "igi.h"

#include "../core/seidennki.h"
#include "light.h"
#include "reflection.h"
#include "sampler.h"
#include "geometry.h"
#include "renderer.h"
void IGIIntegrator::RequestSamples(Sampler *sampler, Sample *sample,
		const Scene *scene) {
	int numLights = scene->getLightNum();
	mLightSampleOffsets = new LightSampleOffsets[numLights];
	mBsdfSampleOffsets = new BSDFSampleOffsets[numLights];
	for (int i = 0; i < numLights; ++i) {
		Light* light = scene->getLight(i);
		int sampleNum = light->numSamples;
		if (sampler)
			sampleNum = sampler->RoundSize(sampleNum);
		mLightSampleOffsets[i] = LightSampleOffsets(sampleNum, sample);
		mBsdfSampleOffsets[i] = BSDFSampleOffsets(sampleNum, sample);
	}
	mVirtualLightSetOffset = sample->Add1D(1); //需要一个样本点来选择一个虚拟光源集合

	if (sampler)
		mNumGatherSamples = sampler->RoundSize(mNumGatherSamples);
	gatherSampleOffset = BSDFSampleOffsets(mNumGatherSamples, sample);
}

void IGIIntegrator::Preprocess(const Scene *scene, const Camera *camera,
		const Renderer *renderer) {
	if (scene->getLightNum() == 0)
		return; //没有光源就不用计算积分器了
	MemoryArena arena;
	Random random;

	vector<float> lightNum(mNumLightPaths * mNumLightSets); //选择光源的样本
	vector<float> lightSampPos(2 * mNumLightPaths * mNumLightSets, 0.f); //光源位置样本,u,v两个
	vector<float> lightSampComp(mNumLightPaths * mNumLightSets, 0.f); //光源Comp样本
	vector<float> lightSampDir(2 * mNumLightPaths * mNumLightSets, 0.f);
	LDShuffleScrambled1D(mNumLightPaths, mNumLightSets, &lightNum[0], random); //采样光源NO 一维
	LDShuffleScrambled2D(mNumLightPaths, mNumLightSets, &lightSampPos[0],
			random); //采样光源位置 二维
	LDShuffleScrambled1D(mNumLightPaths, mNumLightSets, &lightSampComp[0],
			random);
	LDShuffleScrambled2D(mNumLightPaths, mNumLightSets, &lightSampDir[0],
			random); //采样光源方向 二维

	Distribution1D* lightDistribution = ComputeLightSamplingCDF(scene); //生成采样场景光源的CDF
	for (int s = 0; s < mNumLightSets; ++s) {
		for (int i = 0; i < mNumLightPaths; ++i) {
			int sampOffset = mNumLightPaths * s + i; //样本的offset
			float lightPdf; //选取莫个光源的PDF
			int ln = lightDistribution->SampleDiscrete(lightNum[1], &lightPdf);
			Light* light = scene->getLight(ln); //获得光源
			//采样射线
			RayDifferential ray;
			float pdf;
			LightSample ls(lightSampPos[2 * sampOffset],
					lightSampPos[2 * sampOffset + 1],
					lightSampComp[sampOffset]);
			Normal Nl;
			RGB alpha = light->Sample_L(scene, ls, lightSampDir[2 * sampOffset],
					lightSampDir[2 * sampOffset + 1], &ray, &Nl, &pdf);
			//如果采样的光线并没有贡献，就跳过这个路径
			if (pdf == 0.f || alpha.IsBlack())
				continue;
			alpha *= AbsDot(Nl, ray.d) / (pdf * lightPdf);		//计算蒙特卡洛的term

			Intersection isect;
			//开始生成Light Path
			while (scene->Intersect(ray, &isect) && !alpha.IsBlack()) {
				//TODO IGI散射代码未完成
				Vector wo = -ray.d; //出射方向
				BSDF *bsdf = isect.GetBSDF(ray, arena); //获取bsdf
				RGB contrib = alpha * bsdf->rho(wo, random) / Pi;
				//cout << "------" << endl;
				//	RGB contrib(10,10,0);
				//在交点处创建虚拟光源
				mVirtualLights[s].push_back(
						VirtualLight(isect.dg.p, isect.dg.nn, contrib,
								isect.rayEpsilon));
				//开始采样新的射线
				Vector wi;
				float pdf;
				BSDFSample bsdfSample(random);
				RGB fr = bsdf->Sample_f(wo, &wi, bsdfSample, &pdf);
				//如果BRDF没有贡献，光源路径就结束了
				if (fr.IsBlack() || pdf == 0.f)
					break;
				RGB contribScale = fr * AbsDot(wi, bsdf->dgShading.nn) / pdf;
				//俄罗斯转盘继续概率
				float rrProb = min(1.f, contribScale.luminance());
				if (random.RandomFloat() > rrProb)
					break;

				alpha *= contribScale / rrProb; //俄罗斯转盘补正
				ray = RayDifferential(isect.dg.p, wi, ray, isect.rayEpsilon); //生成射线
			}
			arena.FreeAll();
		}
	}

	delete lightDistribution;
}

RGB IGIIntegrator::Li(const Scene *scene, const Renderer *renderer,
		const RayDifferential &r, const Sample *sample,
		const Intersection &isect, Random &rnd, MemoryArena& arena) const {

	RGB L(0);
	Vector wo = -r.d;
	L += isect.Le(wo); //添加自发光

	BSDF* bsdf = isect.GetBSDF(r, arena);
	const Point &p = bsdf->dgShading.p;
	const Normal &nl = bsdf->dgShading.nn;
	//采样直接光 MIS策略
	L += UniformSampleAllLights(scene, renderer, arena, p, nl, wo,
			isect.rayEpsilon, bsdf, sample, rnd, mLightSampleOffsets,
			mBsdfSampleOffsets);
	unsigned int lSet = min(
			(unsigned int) (sample->oneD[mVirtualLightSetOffset][0]
					* mNumLightSets), mNumLightSets - 1);
	//遍历虚拟光源
	for (unsigned int i = 0; i < mVirtualLights[lSet].size(); ++i) {
		const VirtualLight &vl = mVirtualLights[lSet][i];
		float d2 = DistanceSqr(p, vl.p);
		Vector wi = Normalize(vl.p - p); //入射方向
		float G = AbsDot(wi, nl) * AbsDot(wi, vl.n) / d2;
		G = min(G, mGeoLimit);
		RGB f = bsdf->f(wo, wi); //采样BRDF
		if (G == 0.f || f.IsBlack())
			continue;
		RGB Llight = f * G * vl.pathContrib / mNumLightPaths;
		RayDifferential connectRay(p, wi, r, isect.rayEpsilon,
				sqrtf(d2) * (1.f - vl.rayEpsilon));
		//Llight *= renderer->Transmittance(scene, connectRay, NULL, rng, arena);
		//一定概率跳过贡献低的光源
		if (Llight.luminance() < mRrThreshold) {
			float continueProbability = 0.1f;
			if (rnd.RandomFloat() > continueProbability)
				continue;
			Llight = Llight / continueProbability;		//RR补正
		}
		//加入虚拟光源的贡献
		if (!scene->IntersectP(connectRay)) {
			L += Llight;
		}

	}
	if (r.depth < maxSpecularDepth) {
		int nSamples = (r.depth == 0) ? mNumGatherSamples : 1;	//根据射线的深度来判断使用多少的样本
		for (int i = 0; i < nSamples; ++i) {
			Vector wi;
			float pdf;
			BSDFSample bsdfSample =
					(r.depth == 0) ?
							BSDFSample(sample, gatherSampleOffset, i) :
							BSDFSample(rnd);	//根据不同的射线深度，选择不同的BSDF样本
			RGB f = bsdf->Sample_f(wo, &wi, bsdfSample, &pdf,
					BxDFType(BSDF_ALL & ~BSDF_SPECULAR)); //采样BSDF
			if (!f.IsBlack() && pdf > 0.f) {
				float maxDist = sqrtf(AbsDot(wi, nl) /mGeoLimit);//射线最大距离
				RayDifferential gatherRay(p, wi, r, isect.rayEpsilon,maxDist);
				Intersection gatherIsect;
				RGB Li = renderer->Li(scene, gatherRay, sample, rnd, arena,&gatherIsect);
				if (Li.IsBlack())
					continue;
				//加入补偿
				float Ggather = AbsDot(wi, nl) * AbsDot(-wi, gatherIsect.dg.nn)
						/ DistanceSqr(p, gatherIsect.dg.p);
				if (Ggather - mGeoLimit > 0.f && !isinf(Ggather)) {
					float gs = (Ggather - mGeoLimit) / Ggather;
					L += f * Li * (AbsDot(wi, nl) * gs / (nSamples * pdf));
				}
			}
		}
	}

	if (r.depth + 1 < maxSpecularDepth) {
		L += SpecularReflect(r, bsdf, rnd, isect, renderer, scene, sample,
				arena);
		//L += SpecularTransmit(r, bsdf, rnd, isect, renderer, scene, sample,
			//			arena);
	}
	return L;
}

