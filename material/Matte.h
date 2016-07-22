//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_MATTE_H
#define RAYTRACER_MATTE_H


#include <material.h>
#include "reflection.h"
#include <memory>
using namespace std;
class Matte : public Material{
private:
    Lambertian mDiffuseBrdf;
    RGB mR;
public:
    Matte(const RGB& _albedo=RGB(1,1,1));
//    virtual RGB shade(const Intersection& sr);
    virtual ~Matte(){}

    virtual BSDF *GetBSDF(const DifferentialGeometry &dgGeom,
                             const DifferentialGeometry &dgShading,
                             MemoryArena &arena) const override;
};


#endif //RAYTRACER_MATTE_H
