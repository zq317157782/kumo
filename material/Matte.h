//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_MATTE_H
#define RAYTRACER_MATTE_H


#include "Material.h"
#include "../brdf/Lambertian.h"
#include <memory>
using namespace std;
class Matte : public Material{
private:
    shared_ptr<Lambertian> mAmbientBrdf;
    shared_ptr<Lambertian> mDiffuseBrdf;
public:
    Matte(const RGB& _albedo=RGB(1,1,1));
    void scaleAmbientAlbedo(const float k);
    void scaleDiffuseAlbedo(const float k);
    void setAlbedo(const RGB& color);

    virtual RGB shade(ShadeRec& sr);
    virtual RGB areaLightShade(ShadeRec& sr);
};


#endif //RAYTRACER_MATTE_H
