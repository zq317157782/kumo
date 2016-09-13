/*
 * lowdiscrepancy.cpp
 *
 *  Created on: 2016年9月13日
 *      Author: zhuqian
 */
#include "lowdiscrepancy.h"
#include "montecarlo.h"

LDSampler::LDSampler(int xstart/*x开始坐标*/, int xend/*x结束坐标*/,
		int ystart/*y开始坐标*/, int yend/*y结束坐标*/, int nSample) :
		Sampler(xstart, xend, ystart, yend, RoundUpPow2(nSample)) {
	xPos = xPixelStart;
	yPos = yPixelStart;
	if (!IsPowerOf2(nSample)) {
		nPixelSamples = RoundUpPow2(nSample);
	} else {
		nPixelSamples = nSample;
	}
	sampleBuf = nullptr;
}

Sampler* LDSampler::GetSubSampler(int num, int count) {
	int x0, x1, y0, y1;
	ComputeSubWindow(num, count, &x0, &x1, &y0, &y1);
	if (x0 == x1 || y0 == y1) {
		return nullptr;
	}
	return new LDSampler(x0, x1, y0, y1, nPixelSamples);
}

int LDSampler::GetMoreSamples(Sample *sample, Random &rng) {
	if (yPos == yPixelEnd)
		return 0;
	if (sampleBuf == nullptr) {
		sampleBuf = new float[LDPixelSampleFloatsNeeded(sample, nPixelSamples)];
	}
	LDPixelSample(xPos, yPos, nPixelSamples, sample, sampleBuf, rng); //生成采样点
	if (++xPos == xPixelEnd) {
		xPos = xPixelStart;
		++yPos;
	}
	return nPixelSamples;
}
