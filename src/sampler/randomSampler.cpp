/*
 * RandomSampler.cpp
 *
 *  Created on: 2016年7月3日
 *      Author: zhuqian
 */

#include "memory.h"
#include "randomSampler.h"
#include "random.h"

int RandomSampler::RoundSize(int size) const{
	return size;
}

RandomSampler::RandomSampler(int xStart, int xEnd, int yStart, int yEnd,
		int spp) :
		Sampler(xStart, xEnd, yStart, yEnd, spp) {
	xPos = xStart;
	yPos = yStart;
	nSamples = spp;
	imageSamples = AllocAligned<float>(4 * nSamples); //为一个像素的所有采样点分配空间 2->image 2->lens
	lensSamples = imageSamples + 2 * nSamples;

	Random rand(xStart + yStart * (xEnd - xStart));
	for (int i = 0; i < 4 * nSamples; ++i) {
		imageSamples[i] = rand.RandomFloat();
	}
	//偏移image像素的位置
	for (int j = 0; j < 2 * nSamples; j += 2) {
		imageSamples[j] += xPos;
		imageSamples[j + 1] += yPos;
	}
	samplePos = 0;
}

int RandomSampler::GetMoreSamples(Sample *sample, Random &rng) {
	if (samplePos == nSamples) { //1个像素内需要生成的采样点全部生成后，开始下个像素
		if (xPixelStart == xPixelEnd || yPixelStart == yPixelEnd) //不需要采样的情况
			return 0;
		if (++xPos == xPixelEnd) {
			xPos = xPixelStart;
			++yPos;
		}
		if (yPos == yPixelEnd)
			return 0;
		for (int i = 0; i < 4 * nSamples; ++i)
			imageSamples[i] = rng.RandomFloat();
		for (int j = 0; j < 2 * nSamples; j += 2) {
			imageSamples[j] += xPos;
			imageSamples[j + 1] += yPos;
		}
		samplePos = 0;
	}

	sample->imageX = imageSamples[2 * samplePos];
	sample->imageY = imageSamples[2 * samplePos + 1];
	sample->lensU = lensSamples[2 * samplePos];
	sample->lensV = lensSamples[2 * samplePos + 1];
	//生成额外的1维采样点
	for (unsigned int i = 0; i < sample->n1D.size(); ++i)
		for (unsigned int j = 0; j < sample->n1D[i]; ++j)
			sample->oneD[i][j] = rng.RandomFloat();
	//生成额外的2维采样点
	for (unsigned int i = 0; i < sample->n2D.size(); ++i)
		for (unsigned int j = 0; j < 2 * sample->n2D[i]; ++j)
			sample->twoD[i][j] = rng.RandomFloat();

	++samplePos;
	return 1;
}

Sampler* RandomSampler::GetSubSampler(int num, int count) {
	int x0, x1, y0, y1;
	ComputeSubWindow(num, count, &x0, &x1, &y0, &y1);
	if (x0 == x1 || y0 == y1)
		return nullptr;
	return new RandomSampler(x0, x1, y0, y1, nSamples);
}

RandomSampler::~RandomSampler() {

}

