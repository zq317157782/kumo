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
protected:
	 void ComputeSubWindow(int num, int count, int *xstart, int *xend, int *ystart, int *yend) const;//用来计算一个子窗口的位置，以及大小
};


//相机采样点
struct CameraSample {
    float imageX, imageY;
    float lensU, lensV;
};

//普通采样点
struct Sample:public CameraSample{
public:
	 vector<unsigned int> n1D, n2D;
	 float **oneD, **twoD; //这里是存放真正的采样点的地方

	 Sample(Sampler* sampler,SurfaceIntegrator* si,const Scene* scene);

	 //额外的1维采样点
	 unsigned int Add1D(unsigned int num) {
	     n1D.push_back(num);
	     return n1D.size()-1;
	 }

	 //额外的二维采样点
	 unsigned int Add2D(unsigned int num) {
	         n2D.push_back(num);
	         return n2D.size()-1;
	 }

	 //释放所有的动态申请的空间
	 ~Sample();

	 //复制采样点
	 Sample *Duplicate(int count) const;

private:
    void AllocateSampleMemory(); //分配存放采样点的内存
    Sample() { oneD = twoD = nullptr; }
};

#endif /* CORE_SAMPLER_H_ */
