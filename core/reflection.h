//
// Created by Administrator on 2016/6/28 0028.
//

#ifndef RAYTRACER_REFLECTION_H
#define RAYTRACER_REFLECTION_H

#include "global.h"

//反射坐标系 三个标准正交基是两切线和法线

//cos(t)=(N.DOT.w)==(0,0,1).dot.w=w.z
inline float CosTheta(const Vector &w){
    return w.z;
}
inline float AbsCosTheta(const Vector &w) {
    return fabsf(w.z);
}

//sin(t)2+cos(t)2==1
inline float SinTheta2(const Vector& w){
    float cosw=CosTheta(w);
    return max(0.0f,1.0f-cosw*cosw);
}
inline float SinTheta(const Vector& w){
    return sqrtf(SinTheta2(w));
}


#endif //RAYTRACER_REFLECTION_H
