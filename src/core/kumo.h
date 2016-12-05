//
// Created by 诸谦 on 16/6/10.
//

#ifndef RAYTRACER_GLOBAL_H
#define RAYTRACER_GLOBAL_H


#include <iostream>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
using namespace std;

//定义一个Float宏 可能指向float可能指向double
#ifdef FLOAT_IS_DOUBLE
typedef double Float;
#else
typedef float Float;
#endif

//无限大数
static constexpr Float Infinity = std::numeric_limits<Float>::infinity();

//根据编译选项设置断言宏
#ifdef DEBUG_BUILD
#define Assert(x) assert(x)
#else
#define Assert(x) ((void)0)
#endif

//打印错误的宏定义，我把它定义在这，PBRT的实现我没有深究，对我来说，这个目前已经做够了
#define Error(x) std::cerr<<x<<std::endl;

static constexpr Float Pi = 3.14159265358979323846;
static constexpr Float InvPi=1.0f/Pi;
static constexpr Float TwoPi=2.0f*Pi;
static constexpr Float InvTwoPi=1.0f/TwoPi;

#define ALLOCA(TYPE, COUNT) (TYPE *)alloca((COUNT) * sizeof(TYPE))

class Vector; //向量
class Point;  //空间点
class Normal; //法线
class Ray; //射线
class RayDifferential;
class Primitive; //图元
class Shape;  //几何体
class Transform; //变换
class Material; //材质
struct DifferentialGeometry; //微分几何
class Scene;//场景
class Renderer;//渲染器
class Camera;//相机
class RGB;//颜色
class Integrator;
class SurfaceIntegrator;
class Intersection;
class Random;//随机数
struct Sample;//采样点
struct CameraSample;//相机采样点
class Sampler; //采样器
class Filter; //过滤器
class Film;
class Light;//光源
class AreaLight;//区域光
struct LightSample;//光源采样点
struct LightSampleOffsets;
struct VisibilityTester;
class Microfacet; //微平面
class MicrofacetDistribution;//微平面法线分布函数
class Fresnel;//菲涅尔系数
class FresnelConductor;//导电体Fresnel
class BSDF;	//整合BxDF的结构
class MemoryArena;//内存分配区域
template<typename T> class Texture; //纹理
class TextureMapping2D;//纹理映射
struct BSDFSample; //BSDF样本
struct BSDFSampleOffsets;
struct Distribution1D;
class RWMutex; //读写锁
template<typename T> class MIPMap; //纹理


//float类型相应的IEEE标准的BIT格式
//最高位是符号位，然后8位是指数,接下来23位是值，
//指数为0的时候没有默认最高位的1
//指数为255的时候，值为0则为无穷大，值不为0则为NaN
inline uint32_t FloatToBits(float f) {
	Assert(!std::isnan(f));
	uint32_t bits = 0;
	std::memcpy(&bits, &f, sizeof(float));
	return bits;
}
//从BIT形式转换会float类型
inline float BitsToFloat(uint32_t bits) {
	float f = 0;
	std::memcpy(&f, &bits, sizeof(uint32_t));
	return f;
}

//double版本的 FloatToBits
inline uint64_t FloatToBits(double d) {
	Assert(!std::isnan(d));
	uint64_t bits = 0;
	std::memcpy(&bits, &d, sizeof(double));
	return d;
}

inline double BitsToFloat(uint64_t bits) {
	double d = 0;
	std::memcpy(&d, &bits, sizeof(uint64_t));
	return d;
}

//获取下一个大于本float变量的float变量
//1.先判断是否是无限值，是的话直接返回
//2.如果是负0的话，先转换成正0，因为下面的比较需要0是一个正0
//3.转换成BIT形式，并且比较BIT是否大于0，大于++，小于--
//4.再度转换回float，并且返回
inline float NextFloatUp(float f) {
	Assert(!std::isnan(f));
	if (isinf(f) && f > 0.0f)
		return f;
	if (f == -0.0f)
		f = 0.0f;
	uint32_t bits = FloatToBits(f);
	if (f >= 0)
		++bits;
	else
		--bits;
	return BitsToFloat(bits);
}

//获取下一个小于本float变量的float变量
inline float NextFloatDown(float f) {
	Assert(!std::isnan(f));
	if (isinf(f) && f < 0.0f)
		return f;
	if (f == 0.0f)
		f = -0.0f;
	uint32_t bits = FloatToBits(f);
	if (f > 0)
		--bits;
	else
		++bits;
	return BitsToFloat(bits);
}

inline double NextFloatUp(double d, int delta = 1) {
	Assert(!std::isnan(d));
	if (isinf(d) && d > 0.0)
		return d;
	if (d == -0.0)
		d = 0.0;
	uint64_t bits = FloatToBits(d);
	if (d >= 0)
		bits += delta;
	else
		bits -= delta;
	return BitsToFloat(bits);
}

inline double NextFloatDown(double d, int delta = 1) {
	Assert(!std::isnan(d));
	if (isinf(d) && d < 0.0)
		return d;
	if (d == 0.0)
		d = -0.0;
	uint64_t bits = FloatToBits(d);
	if (d > 0)
		bits -= delta;
	else
		bits += delta;
	return BitsToFloat(bits);
}

//这个MachineEpsion是数值分析下的MachineEpsion；为2的-24次方；
//C++标准库提供的Epsion是大于1的ULP，为2的-23次方
//所以需要再除以2
static constexpr Float MachineEpsion = 0.5f
		* std::numeric_limits<Float>::epsilon();

//这个gamma不是用来做Gamma校正的gamma，这个gamma是浮点数运算中，每一次运算后的最大ERR边界
inline constexpr Float gamma(int n) {
	return (n * MachineEpsion) / (1 - n * MachineEpsion);
}

//裁剪函数
inline Float Clamp(Float val, Float low, Float high) {
	if (val < low)
		return low;
	else if (val > high)
		return high;
	else
		return val;
}

//线性插值
inline Float Lerp(Float val, Float min, Float max) {
	return min + (max - min) * val;
}

//角度转换弧度
inline Float Radians(Float deg) {
	return (Pi / 180) * deg;
}

//寻找区间 返回offset
//使用二分法来寻找区间,区间为offset~offset+1
//保证值有效的情况下，val[offset]满足预测函数，val[offset]不满足预测函数
//如果值不在提供的范围中，就会返回最前面或者最后的两个区间中的一个
template<typename PredicateFunc>
int FindInterval(int size, const PredicateFunc &pred) {
	int first = 0, len = size;
	while (len > 0) {
		int half = len >> 1, middle = first + half;
		if (pred(middle)) {
			first = middle + 1;
			len -= half + 1;
		} else
			len = half;
	}
	return Clamp(first - 1, 0, size - 2);
}



//求解二次项方程
inline  bool Quadratic(float A,float B,float C,float* t0,float *t1){
    // 计算判别式
    float discrim = B * B - 4.f * A * C;
    if (discrim < 0.) return false;
    float rootDiscrim = sqrtf(discrim);

    //这是另一种代数方式求根,PBRT上说用传统的方式会有误差当B的值接近delta,应该是浮点数造成的
    float q;
    if (B < 0) q = -.5f * (B - rootDiscrim);
    else       q = -.5f * (B + rootDiscrim);
    *t0 = q / A;
    *t1 = C / q;
    if (*t0 > *t1) swap(*t0, *t1);
    return true;
}






//向下取整
inline int Floor2Int(float val) {
    return (int)floorf(val);
}

//向上取整
inline int Ceil2Int(float val) {
    return (int)ceilf(val);
}

inline unsigned int RoundUpPow2(uint32_t v) {
    v--;
    v |= v >> 1;    v |= v >> 2;
    v |= v >> 4;    v |= v >> 8;
    v |= v >> 16;
    return v+1;
}

//上取整
inline int Round2Int(float val) {
    return Floor2Int(val + 0.5f);
}

inline int Float2Int(float val) {
    return (int)val;
}

inline bool IsPowerOf2(int v) {
    return v && !(v & (v - 1));
}


inline int Mod(int a, int b) {
    int n = int(a/b);
    a -= n*b;
    if (a < 0) a += b;
    return a;
}

inline float Log2(float x) {
    static float invLog2 = 1.f / logf(2.f);
    return logf(x) * invLog2;
}

inline int Log2Int(float v) {
    return Floor2Int(Log2(v));
}

inline float Degrees(float rad) {
    return (180.f/(float)Pi) * rad;
}



template <typename T, int logBlockSize = 2> class BlockedArray;
#endif //RAYTRACER_GLOBAL_H
