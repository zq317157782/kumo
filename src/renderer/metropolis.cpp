/*
 * metropolis.cpp
 *
 *  Created on: 2016年10月10日
 *      Author: zhuqian
 */
#include "metropolis.h"

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
	vector<LightSample> lightingSamples; //用于相机路径下 计算直接光照的MIS样本

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
		ls.lightNum=rng.RandomFloat();
		ls.lightSample.uComponent= rng.RandomFloat();
		ls.lightSample.uPos[0]=rng.RandomFloat();
		ls.lightSample.uPos[1]=rng.RandomFloat();
	}
	//如果使用了双向路径追踪,还需要整ligthPath的样本
	if(bidirectional){
		sample->lightNumSample=rng.RandomFloat();
		for(int i=0;i<5;++i){
			sample->lightRaySamples[i]=rng.RandomFloat();
		}
		for(int i=0;i<maxDepth;++i){
			PathSample &lps=sample->lightPathSamples[i];
			lps.bsdfSample.uComponent=rng.RandomFloat();
			lps.bsdfSample.uDir[0]=rng.RandomFloat();
			lps.bsdfSample.uDir[1]=rng.RandomFloat();
			lps.rrSample=rng.RandomFloat();
		}
	}
}

