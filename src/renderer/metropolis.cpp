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

