//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_MATTE_H
#define RAYTRACER_MATTE_H


#include <material.h>
#include "reflection.h"
#include "memory.h"
#include "texture.h"
using namespace std;
class Matte : public Material{
private:
    Reference<Texture<RGB>> mR;
public:
    Matte(const Reference<Texture<RGB>>& _albedo);
    virtual ~Matte(){}

    virtual BSDF *GetBSDF(const DifferentialGeometry &dgGeom,
                             const DifferentialGeometry &dgShading,
                             MemoryArena &arena) const override;
};


#endif //RAYTRACER_MATTE_H
