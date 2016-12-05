/*
 * lowdiscrepancy.h
 *
 *  Created on: 2016年9月13日
 *      Author: zhuqian
 */

#ifndef SRC_SAMPLER_LOWDISCREPANCY_H_
#define SRC_SAMPLER_LOWDISCREPANCY_H_
#include "sampler.h"
#include "../core/seidennki.h"
//低差异采样
class LDSampler: public Sampler {
public:
	LDSampler(int xstart/*x开始坐标*/, int xend/*x结束坐标*/, int ystart/*y开始坐标*/,
			int yend/*y结束坐标*/, int nSample);
	virtual ~LDSampler() {
	}
	virtual int MaximumSampleCount() override {
		return nPixelSamples;
	}
	virtual Sampler* GetSubSampler(int num, int count) override;
	virtual int GetMoreSamples(Sample *sample, Random &rng) override;
	virtual int RoundSize(int size) const {
		return RoundUpPow2(size);
	}
	;
private:
	int xPos, yPos, nPixelSamples;
	float *sampleBuf; //临时缓冲区
};

#endif /* SRC_SAMPLER_LOWDISCREPANCY_H_ */
