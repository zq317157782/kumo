//
// Created by 诸谦 on 15/12/25.
//

#ifndef RAYTRACER_BRDF_H
#define RAYTRACER_BRDF_H

#include "global.h"
#include "RGB.h"
#include "../common/ShadeRec.h"

class BRDF {
public:
    virtual RGB f(const ShadeRec& sr, const Vector & wi, const Vector & wo)=0;//计算brdf模型中f系数的函数
    virtual RGB rho(const ShadeRec& sr,const Vector & wo)=0; //计算brdf模型能量转换系数
};


#endif //RAYTRACER_BRDF_H
