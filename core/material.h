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
    virtual RGB shade(const Intersection& sr)=0;

    virtual BSDF *GetBSDF(const DifferentialGeometry &dgGeom,
                             const DifferentialGeometry &dgShading,
                             MemoryArena &arena) const = 0;
};


#endif //RAYTRACER_MATERIAL_H
