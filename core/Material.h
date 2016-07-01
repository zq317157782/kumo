//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H


#include <Intersection.h>
#include "RGB.h"
#include "memory.h"

class Material :public ReferenceCounted{
public:
    virtual RGB shade(Intersection& sr)=0;
    virtual RGB areaLightShade(Intersection& sr)=0; //区域光shade
};


#endif //RAYTRACER_MATERIAL_H
