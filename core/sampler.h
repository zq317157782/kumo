/*
 * sampler.h
 *
 *  Created on: 2016年7月1日
 *      Author: Administrator
 */

#ifndef CORE_SAMPLER_H_
#define CORE_SAMPLER_H_

#include "global.h"
//todo 新的采样器编写
class Sampler{
public:
	const int xPixelStart,xPixelEnd,yPixelStart,yPixelEnd;
	const int samplesPerPixel; //每个像素包含的采样点个数

	Sampler(int xs,int xe,int ys,int ye,int spp):xPixelStart(xs),xPixelEnd(xe),yPixelStart(ys),yPixelEnd(ye),samplesPerPixel(spp){}
	virtual int GetMoreSamples(Sample* sample,Random& rand)=0; //返回采样点个数  和  采样点数组
	virtual ~Sampler(){}

	 virtual bool ReportResults(Sample *samples, const RayDifferential *rays,
	        const RGB *Ls, const Intersection *isects, int count){return true;};

	 virtual Sampler* GetSubSampler(int num,int count)=0; //返回子采样器数组

};


//相机采样点
struct CameraSample {
    float imageX, imageY;
    float lensU, lensV;
};

//普通采样点
struct Sample:public CameraSample{

};

#endif /* CORE_SAMPLER_H_ */
