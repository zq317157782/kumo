//
// Created by 诸谦 on 15/12/25.
//

#ifndef RAYTRACER_BRDF_H
#define RAYTRACER_BRDF_H

#include <Intersection.h>
#include "global.h"
#include "RGB.h"


class BRDF {
public:
    virtual RGB f(const Intersection& sr, const Vector & wi, const Vector & wo)=0;//返回BRDF系数
    virtual RGB rho(const Intersection& sr,const Vector & wo)=0; //返回反射系数
};


#endif //RAYTRACER_BRDF_H
