/*
 * stratified.h
 *
 *  Created on: 2016年8月9日
 *      Author: zhuqian
 */

#ifndef SAMPLER_STRATIFIED_H_
#define SAMPLER_STRATIFIED_H_
#include "sampler.h"
#include "../core/seidennki.h"
 //分层采样
class StratifiedSampler : public Sampler {
public:
	StratifiedSampler(int xstart/*x开始坐标*/, int xend/*x结束坐标*/,
		int ystart/*y开始坐标*/, int yend/*y结束坐标*/, int xs/*x方向的样本数*/,
		int ys/*y方向的样本数*/, bool jitter/*是否需要抖动*/);
	virtual ~StratifiedSampler() {
	}
	virtual int MaximumSampleCount() override { return xPixelSamples*yPixelSamples; }
	virtual Sampler* GetSubSampler(int num, int count) override;
	virtual int GetMoreSamples(Sample *sample, Random &rng) override;
	virtual int RoundSize(int size) const override;
private:
	int xPixelSamples, yPixelSamples;//x和y方向上的样本数
	bool jitterSamples;//是否抖动
	int xPos, yPos;
	Float *sampleBuf;//临时缓冲区
};
#endif // !SAMPLER_STRATIFIED_H_
