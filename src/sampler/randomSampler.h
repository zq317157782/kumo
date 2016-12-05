/*
 * RandomSampler.h
 *
 *  Created on: 2016年7月3日
 *      Author: zhuqian
 */

#ifndef SAMPLER_RANDOMSAMPLER_H_
#define SAMPLER_RANDOMSAMPLER_H_
#include "../core/seidennki.h"
#include "sampler.h";

class RandomSampler :public Sampler{
public:
	RandomSampler(int xStart,int xEnd,int yStart,int yEnd,int spp);
	virtual ~RandomSampler();
	virtual int MaximumSampleCount() override{ return 1; }
	virtual int GetMoreSamples(Sample *sample, Random &rng) override;
	virtual Sampler* GetSubSampler(int num,int count) override;
	virtual int RoundSize(int size) const override;
private:
    int xPos, yPos, nSamples;
    Float *imageSamples, *lensSamples;
    int samplePos;
};

#endif /* SAMPLER_RANDOMSAMPLER_H_ */
