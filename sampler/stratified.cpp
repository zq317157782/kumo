/*
 * stratified.cpp
 *
 *  Created on: 2016年8月9日
 *      Author: zhuqian
 */
#include "stratified.h"
#include "montecarlo.h"

StratifiedSampler::StratifiedSampler(int xstart, int xend, int ystart, int yend,
		int xs, int ys, bool jitter) :
		Sampler(xstart, xend, ystart, yend, xs * ys) {
	xPos = xstart;
	yPos = ystart;
	xPixelSamples = xs;
	yPixelSamples = ys;
	jitterSamples = jitter;
	sampleBuf = new float[4 * xPixelSamples * yPixelSamples]; //一个像素内所有的采样点的临时空间
}

Sampler* StratifiedSampler::GetSubSampler(int num, int count) {
	int x0, x1, y0, y1;
	ComputeSubWindow(num, count, &x0, &x1, &y0, &y1);
	if (x0 == x1 || y0 == y1)
		return nullptr;
	return new StratifiedSampler(x0, x1, y0, y1, xPixelSamples, yPixelSamples,
			jitterSamples);
}

int StratifiedSampler::GetMoreSamples(Sample *sample, Random &rand) {
	if (yPos == yPixelEnd)
		return 0; //采样完毕
	int nSamples = xPixelSamples * yPixelSamples;

	//创建指向相应缓冲区地址的指针
	float *bufPointer = sampleBuf;
	float *imageSamples = bufPointer;
	bufPointer += 2 * nSamples;
	float *lensSamples = bufPointer;
	bufPointer += 2 * nSamples;

	StratifiedSample2D(imageSamples, xPixelSamples, yPixelSamples, rand);
	StratifiedSample2D(lensSamples, xPixelSamples, yPixelSamples, rand);

	for (int i = 0; i < nSamples * 2; i += 2) {
		imageSamples[i] += xPos;
		imageSamples[i + 1] += yPos;
	}
	Shuffle(lensSamples, nSamples, 2, rand); //打乱len采样点

	for (int i = 0; i < nSamples; ++i) {
		sample[i].imageX = imageSamples[2 * i];
		sample[i].imageY = imageSamples[2 * i + 1];
		sample[i].lensU = lensSamples[2 * i];
		sample[i].lensV = lensSamples[2 * i + 1];

		//生成积分器需要的1维样本和2维样本
		for (unsigned int j = 0; j < sample[i].n1D.size(); ++j)
			LatinHypercube(sample[i].oneD[j], sample[i].n1D[j], 1, rand);
		for (unsigned int  j = 0; j < sample[i].n2D.size(); ++j)
			LatinHypercube(sample[i].twoD[j], sample[i].n2D[j], 2, rand);
	}

	//判断是否换行
	if (++xPos == xPixelEnd) {
		xPos = xPixelStart;
		++yPos;
	}
	return nSamples;
}
