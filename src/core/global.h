//
// Created by 诸谦 on 16/6/10.
//

#ifndef RAYTRACER_GLOBAL_H
#define RAYTRACER_GLOBAL_H

#define CORE_NUM 4

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
using namespace std;


//如果平台没有定义M_PI的话  定义M_PI
#ifndef  M_PI
#define  M_PI        3.14159265358979323846

//drand48函数  返回double型的随机数，如果平台已经有这个函数 需要把这里注解掉

#define MNWZ 0x100000000
#define ANWZ 0x5DEECE66D
#define CNWZ 0xB16
static unsigned long long seed = 1;
inline double drand48(){
    seed = (ANWZ * seed + CNWZ) & 0xFFFFFFFFFFFFLL;
    unsigned int x = seed >> 16;
    return ((double)x / (double)MNWZ);
}

#endif

#ifndef M_INV_PI
#define M_INV_PI  1/M_PI
#endif
#define  M_TWO_PI    2*M_PI
#define M_INV_TWO_PI  1/M_TWO_PI


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


//角度到弧度的转换
inline float Radians(float deg) {
    return ((float)M_PI/180.f) * deg;
}

//裁剪函数
inline float Clamp(float val, float low, float high) {
    if (val < low) return low;
    else if (val > high) return high;
    else return val;
}

//向下取整
inline int Floor2Int(float val) {
    return (int)floorf(val);
}

//向上取整
inline int Ceil2Int(float val) {
    return (int)ceilf(val);
}


//插值
inline float Lerp(float t, float v1, float v2) {
    return (1.f - t) * v1 + t * v2;
}

inline unsigned int RoundUpPow2(uint32_t v) {
    v--;
    v |= v >> 1;    v |= v >> 2;
    v |= v >> 4;    v |= v >> 8;
    v |= v >> 16;
    return v+1;
}

#endif //RAYTRACER_GLOBAL_H
