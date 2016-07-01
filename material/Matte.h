//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_MATTE_H
#define RAYTRACER_MATTE_H


#include "Material.h"
#include "reflection.h"
#include <memory>
using namespace std;
class Matte : public Material{
private:
    Lambertian mAmbientBrdf;
    Lambertian mDiffuseBrdf;
public:
    Matte(const RGB& _albedo=RGB(1,1,1));
    virtual RGB shade(const Intersection& sr);
    virtual RGB areaLightShade(Intersection& sr);
};


#endif //RAYTRACER_MATTE_H
