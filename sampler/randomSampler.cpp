/*
 * RandomSampler.cpp
 *
 *  Created on: 2016年7月3日
 *      Author: zhuqian
 */

#include "memory.h"
#include "randomSampler.h"
#include "random.h"

RandomSampler::RandomSampler(int xStart,int xEnd,int yStart,int yEnd,int spp):Sampler(xStart,xEnd,yStart,yEnd,spp){
	xPos = xStart;
	yPos = yStart;
	nSamples = spp;
	imageSamples = AllocAligned<float>(4 * nSamples); //为一个像素的所有采样点分配空间 2->image 2->lens
	lensSamples = imageSamples + 2 * nSamples;

	Random rand(xStart+yStart*(xEnd-xStart));
	for(int i=0;i<4*nSamples;++i){
		imageSamples[i]=rand.RandomFloat();
	}
	//偏移image像素的位置
	for (int j = 0; j < 2 * nSamples; j += 2) {
	        imageSamples[j]   += xPos;
	        imageSamples[j+1] += yPos;
	}
	samplePos = 0;
}

RandomSampler::~RandomSampler() {

}

