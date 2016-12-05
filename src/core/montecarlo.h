/*
 * montecarlo.h
 *
 *  Created on: 2016年7月27日
 *      Author: Administrator
 */

#ifndef CORE_MONTECARLO_H_
#define CORE_MONTECARLO_H_
//这里都是关于蒙特卡洛方法的相关工具

#include <kumo.h>
#include "random.h"
#include "geometry.h"
#include "sampler.h"

static const Float OneMinusEpsilon=0.9999999999999;

//均匀的1维分布 作用域x为0~1
struct Distribution1D {
private:
	int mCount;
	Float* mFunc;
	Float* mCDF;
	Float mFuncInt;
public:
	/*
	 *data 数据数组
	 *n    数据个数
	 * */
	Distribution1D(Float* data, int n) {
		mCount = n;
		mFunc = new Float[mCount];
		memcpy(mFunc, data, sizeof(Float) * mCount);
		mCDF = new Float[mCount + 1];

		//这里面一开始放的是相应积分域上的积分值
		mCDF[0] = 0;
		for (int i = 1; i < mCount + 1; ++i) {
			mCDF[i] = mFunc[i - 1] / mCount + mCDF[i - 1]; //注意，这里是积分值，还没有标准化
		}

		mFuncInt = mCDF[mCount]; //作用域上的总的积分值

		//下面是标准化操作 使得CDF下面存放的是累积概率
		if (mFuncInt == 0) {
			for (int i = 0; i < mCount + 1; ++i)
				mCDF[i] = Float(i) / Float(mCount);
		} else {
			for (int i = 0; i < mCount + 1; ++i)
				mCDF[i] /= mFuncInt;
		}

	}

	~Distribution1D() {
		delete[] mFunc;
		delete[] mCDF;
	}

	/*
	 * u 0~1之间的   pdf概率  off偏移
	 * */
	Float SampleContinuous(Float u, Float* pdf, int *off) const {
		Float *ptr = std::upper_bound(mCDF, mCDF + mCount + 1, u);
		int offset = std::max(0, int(ptr - mCDF - 1)); //offset是小于u的cdf所在的偏移
		if (off)
			*off = offset;
		Float du = (u - mCDF[offset]) / (mCDF[offset + 1] - mCDF[offset]); //u在cdf[offset+1]-cdf[offset]之间的所在位置百分比

		if (pdf)
			*pdf = mCDF[offset] / mFuncInt; //计算offset下的概率密度
		return (offset + du) / mCount; //计算连续的返回值，在0~1之间
	}

	//采样离散版本
	int SampleDiscrete(Float u, Float* pdf) const{
		Float *ptr = std::upper_bound(mCDF, mCDF + mCount + 1, u);
		int offset = std::max(0, int(ptr - mCDF - 1)); //offset是小于u的cdf所在的偏移
		if (pdf) *pdf = mFunc[offset] / (mFuncInt * mCount);
		return offset;
	}
};

//使用拒绝采样方法来采样圆盘
void RejectionSampleDisk(Float* x, Float*y, Random& rand);

//采样半球
//这里采用的是采样多维分布的方法
Vector UniformSampleHemisphere(Float u1, Float u2);


//返回Uniform采样半球的pdf
inline Float UniformHemispherePdf() {
	return InvTwoPi;
}

//采样球
Vector UniformSampleSphere(Float u1, Float u2);

//返回采样球的PDF
inline Float UniformSpherePdf(){
	return 1.0f/(4.0f*Pi);
}


//均匀采样Disk
void UniformSampleDisk(Float u1,Float u2,Float *x,Float *y);



//TODO 同轴Disk采样是复制的
//同轴采样DISK 直接复制自PBRT
void ConcentricSampleDisk(Float u1, Float u2, Float *dx, Float *dy) ;

//cos分布采样
Vector CosSampleHemisphere(Float u1,Float u2);


//COS(theta)/PI cos分布的半球概率密度函数
inline Float CosHemispherePdf(Float costheta,Float phi){
	return costheta*InvPi;
}



//多重重要性采样中的 控制样本权重的函数
inline Float BalanceHeuristic(int nf,Float fpdf,int ng,Float gpdf){
	return (nf*fpdf)/(nf*fpdf+ng*gpdf);
}

//指数版本的MIS Wight函数,这里指数为2
inline Float PowerHeuristic(int nf,Float fpdf,int ng,Float gpdf){
	Float f=nf*fpdf;
	Float g=ng*gpdf;
	return (f*f)/(f*f+g*g);
}

//1维分层采样
void StratifiedSample1D(Float *samples, int nsamples, Random &rand,
                        bool jitter = true);
//2维分层采样
void StratifiedSample2D(Float *samples, int nx, int ny, Random &rand,
                        bool jitter = true);
//用于打乱样本顺序
template <typename T>
void Shuffle(T *samp, unsigned int count, unsigned int dims, Random &rand) {
    for (unsigned int  i = 0; i < count; ++i) {
    	unsigned int other = i + (rand.RandomUInt() % (count - i));//随机选择一个其他位置
        for (unsigned int j = 0; j < dims; ++j){
			std::swap(samp[dims*i + j], samp[dims*other + j]);
        }
    }
}

//rooking-sampling
void LatinHypercube(Float *samples, unsigned int nSamples, unsigned int nDim, Random &rng);



void Sample02(unsigned int n, const unsigned int scramble[2],
	Float sample[2]);
Float VanDerCorput(unsigned int n, unsigned int scramble);
Float Sobol2(unsigned int n, unsigned int scramble);

//[0-2]序列 二维 生成一个二维样本点,存放在sample[2]中
inline void Sample02(unsigned int n, const unsigned int scramble[2],
                     Float sample[2]) {
    sample[0] = VanDerCorput(n, scramble[0]);
    sample[1] = Sobol2(n, scramble[1]);
}

//VanDerCorput低差异序列 base==2 只操作32位
inline Float VanDerCorput(uint32_t n, uint32_t scramble) {
    //--------------交换 start------------------
    n = (n << 16) | (n >> 16);
    n = ((n & 0x00ff00ff) << 8) | ((n & 0xff00ff00) >> 8);
    n = ((n & 0x0f0f0f0f) << 4) | ((n & 0xf0f0f0f0) >> 4);
    n = ((n & 0x33333333) << 2) | ((n & 0xcccccccc) >> 2);
    n = ((n & 0x55555555) << 1) | ((n & 0xaaaaaaaa) >> 1);
    //--------------交换 end------------------
    n ^= scramble;
    return std::min(((n>>8) & 0xffffff) / Float(1 << 24), OneMinusEpsilon);
}

//Sobol2低差异序列 base==2 只操作32位
inline Float Sobol2(uint32_t n, uint32_t scramble) {
    for (unsigned int v = 1 << 31; n != 0; n >>= 1, v ^= v >> 1)
        if (n & 0x1) scramble ^= v;
    return std::min(((scramble>>8) & 0xffffff) / Float(1 << 24), OneMinusEpsilon);
}

inline void LDShuffleScrambled1D(int nSamples, int nPixel,
                                 Float *samples, Random &rng) {
	unsigned int scramble = rng.RandomUInt();
    for (int i = 0; i < nSamples * nPixel; ++i)
        samples[i] = VanDerCorput(i, scramble);
    for (int i = 0; i < nPixel; ++i)
        Shuffle(samples + i * nSamples, nSamples, 1, rng);
    Shuffle(samples, nPixel, nSamples, rng);
}


inline void LDShuffleScrambled2D(int nSamples, int nPixel,
                                 Float *samples, Random &rng) {
	unsigned int scramble[2] = { rng.RandomUInt(), rng.RandomUInt() };
    for (int i = 0; i < nSamples * nPixel; ++i)
        Sample02(i, scramble, &samples[2*i]);
    for (int i = 0; i < nPixel; ++i)
        Shuffle(samples + 2 * i * nSamples, nSamples, 2, rng);
    Shuffle(samples, nPixel, 2 * nSamples, rng);
}

//计算一个像素内低差异samples总共占据的内存空间大小
int LDPixelSampleFloatsNeeded(const Sample* sample,int numPixelSamples);
//采样LD样本
void LDPixelSample(int xPos,int yPos,int numPixelSamples,Sample* samples,Float * buf,Random rand);
#endif /* CORE_MONTECARLO_H_ */
