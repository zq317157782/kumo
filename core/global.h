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
using namespace std;


class Vector; //向量
class Point;  //空间点
class Normal; //法线
class Ray; //射线
class Shape;  //几何体
class Transform; //变换
class Primitive; //图元
class Material; //材质
struct DifferentialGeometry; //微分几何
class Scene;//场景
class Renderer;//渲染器
class Camera;//相机

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

#endif //RAYTRACER_GLOBAL_H
