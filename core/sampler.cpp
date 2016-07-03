/*
 * sampler.cpp
 *
 *  Created on: 2016年7月1日
 *      Author: Administrator
 */

#include "sampler.h"
#include "global.h"
#include "memory.h"
#include "integrator.h"

void Sampler::ComputeSubWindow(int num, int count, int *newXStart,
        int *newXEnd, int *newYStart, int *newYEnd) const {
    // Determine how many tiles to use in each dimension, _nx_ and _ny_
    int dx = xPixelEnd - xPixelStart, dy = yPixelEnd - yPixelStart;
    int nx = count, ny = 1;
    while ((nx & 0x1) == 0 && 2 * dx * ny < dy * nx) {
        nx >>= 1;
        ny <<= 1;
    }
    assert(nx * ny == count);

    // Compute $x$ and $y$ pixel sample range for sub-window
    int xo = num % nx, yo = num / nx;
    float tx0 = float(xo) / float(nx), tx1 = float(xo+1) / float(nx);
    float ty0 = float(yo) / float(ny), ty1 = float(yo+1) / float(ny);
    *newXStart = Floor2Int(Lerp(tx0, xPixelStart, xPixelEnd));
    *newXEnd   = Floor2Int(Lerp(tx1, xPixelStart, xPixelEnd));
    *newYStart = Floor2Int(Lerp(ty0, yPixelStart, yPixelEnd));
    *newYEnd   = Floor2Int(Lerp(ty1, yPixelStart, yPixelEnd));
}

//采样点构造器
Sample::Sample(Sampler* sampler,SurfaceIntegrator* si,const Scene* scene){
	if(si) si->RequestSamples(sampler,this,scene);
	AllocateSampleMemory();//申请内存空间
}

Sample::~Sample() {
	         if (oneD != nullptr) {
	             FreeAligned(oneD[0]);
	             FreeAligned(oneD);
	         }
	 }

//这里要生成一个存放所有采样点的二维数组
void Sample::AllocateSampleMemory() {
	int nPtrs=n1D.size()+n2D.size();
	if(!nPtrs){ //不用分配额外采样点的情况
		oneD=twoD=nullptr;
		return;
	}

	oneD=AllocAligned<float*>(nPtrs);
	twoD = oneD+n1D.size();//存放二维样本点的二维数组的起始位置

	int totalSamples=0;
	for(unsigned int i=0;i<n1D.size();++i)
		totalSamples+=n1D[i];
	for(unsigned int i=0;i<n2D.size();++i)
		totalSamples+=n2D[i]*2;

	float* mem=AllocAligned<float>(totalSamples);//分配所有采样点的内存空间

	for(unsigned int i=0;i<n1D.size();++i){
		oneD[i]=mem;
		mem+=n1D[i];
	}
	for(unsigned int i=0;i<n2D.size();++i){
		twoD[i]=mem;
		mem+=n2D[i]*2;
	}

}


Sample *Sample::Duplicate(int count) const{
	Sample* ret=new Sample[count];
	for(int i=0;i<count;++i){
		ret[i].n1D=n1D;
		ret[i].n2D=n2D;
		ret[i].AllocateSampleMemory();
	}
	return ret;
}
