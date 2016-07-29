/*
 * montecarlo.h
 *
 *  Created on: 2016年7月27日
 *      Author: Administrator
 */

#ifndef CORE_MONTECARLO_H_
#define CORE_MONTECARLO_H_
//这里都是关于蒙特卡洛方法的相关工具

#include "global.h"

//均匀的1维分布 作用域x为0~1
struct Distribution1D {
private:
	int mCount;
	float* mFunc;
	float* mCDF;
	float mFuncInt;
public:
	/*
	 *data 数据数组
	 *n    数据个数
	 * */
	Distribution1D(float* data, int n) {
		mCount = n;
		mFunc = new float[mCount];
		memcpy(mFunc, data, sizeof(float) * mCount);
		mCDF = new float[mCount + 1];

		//这里面一开始放的是相应积分域上的积分值
		mCDF[0] = 0;
		for (int i = 1; i < mCount + 1; ++i) {
			mCDF[i] = mFunc[i - 1] / mCount + mCDF[i - 1]; //注意，这里是积分值，还没有标准化
		}

		mFuncInt = mCDF[mCount]; //作用域上的总的积分值

		//下面是标准化操作 使得CDF下面存放的是累积概率
		if (mFuncInt == 0) {
			for (int i = 0; i < mCount + 1; ++i)
				mCDF[i] = float(i) / float(mCount);
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
	float SampleContinuous(float u, float* pdf, int *off) const {
		float *ptr = std::upper_bound(mCDF, mCDF + mCount + 1, u);
		int offset = max(0, int(ptr - mCDF - 1)); //offset是小于u的cdf所在的偏移
		if (off)
			*off = offset;
		float du = (u - mCDF[offset]) / (mCDF[offset + 1] - mCDF[offset]); //u在cdf[offset+1]-cdf[offset]之间的所在位置百分比

		if (pdf)
			*pdf = mCDF[offset] / mFuncInt; //计算offset下的概率密度
		return (offset + du) / mCount; //计算连续的返回值，在0~1之间
	}

	//采样离散版本
	int SampleDiscrete(float u, float* pdf) {
		float *ptr = std::upper_bound(mCDF, mCDF + mCount + 1, u);
		int offset = max(0, int(ptr - mCDF - 1)); //offset是小于u的cdf所在的偏移
		if (pdf)
			*pdf = mCDF[offset] / mFuncInt; //计算offset下的概率密度
		return offset;
	}
};

//使用拒绝采样方法来采样圆盘
void RejectionSampleDisk(float* x, float*y, Random& rand) {
	float sx, sy;
	do {
		sx = 1 - 2 * rand.RandomFloat();
		sy = 1 - 2 * rand.RandomFloat();
	} while (sx * sx + sy * sy > 1);
	*x = sx;
	*y = sy;
}

//采样半球
//这里采用的是采样多维分布的方法
Vector UniformSampleHemisphere(float u1, float u2) {
	float z = u1;
	float r = sqrtf(max(0.0f, 1.0f - z * z)); //这里使用max是为了做保护,防止1-z^2小于0
	float phi = 2 * M_PI * u2;
	float x = cosf(phi) * r;
	float y = sinf(phi) * r;
	return Vector(x, y, z);
}

//返回Uniform采样半球的pdf
float UniformHemispherePdf() {
	return M_INV_TWO_PI;
}

//采样球
Vector UniformSampleSphere(float u1, float u2) {
	float z = 1.0f-2.0f*u1;//这里是和UniformSampleHemisphere唯一的区别
	float r = sqrtf(max(0.0f, 1.0f - z * z)); //这里使用max是为了做保护,防止1-z^2小于0
	float phi = 2 * M_PI * u2;
	float x = cosf(phi) * r;
	float y = sinf(phi) * r;
	return Vector(x, y, z);
}

//返回采样球的PDF
float UniformSpherePdf(){
	return 1.0f/(4.0f*M_PI);
}


//均匀采样Disk
void UniformSampleDisk(float u1,float u2,float *x,float *y){
	float r=sqrtf(u1);
	float theta=2*M_PI*u2;
	*x=r*cosf(theta);
	*y=r*sinf(theta);
}


//TODO 同轴Disk采样是复制的
//同轴采样DISK 直接复制自PBRT
void ConcentricSampleDisk(float u1, float u2, float *dx, float *dy) {
    float r, theta;
    float sx = 2 * u1 - 1;
    float sy = 2 * u2 - 1;
    if (sx == 0.0 && sy == 0.0) {
        *dx = 0.0;
        *dy = 0.0;
        return;
    }
    if (sx >= -sy) {
        if (sx > sy) {
            r = sx;
            if (sy > 0.0) theta = sy/r;
            else          theta = 8.0f + sy/r;
        }
        else {
            r = sy;
            theta = 2.0f - sx/r;
        }
    }
    else {
        if (sx <= sy) {
            r = -sx;
            theta = 4.0f - sy/r;
        }
        else {
            r = -sy;
            theta = 6.0f + sx/r;
        }
    }
    theta *= M_PI / 4.f;
    *dx = r * cosf(theta);
    *dy = r * sinf(theta);
}

//cos分布采样
Vector CosSampleHemisphere(float u1,float u2){
	Vector ret;
	ConcentricSampleDisk(u1,u2,&ret.x,&ret.y);//这里其实可以换成任意的uniform disk sample
	ret.z=sqrtf(max(0.0f,1.0f-ret.x*ret.x-ret.y*ret.y));
	return ret;
}


//COS(theta)/PI cos分布的半球概率密度函数
float CosHemispherePdf(float costheta,float phi){
	return costheta*M_INV_PI;
}



//多重重要性采样中的 控制样本权重的函数
float BalanceHeuristic(int nf,float fpdf,int ng,float gpdf){
	return (nf*fpdf)/(nf*fpdf+ng*gpdf);
}


#endif /* CORE_MONTECARLO_H_ */
