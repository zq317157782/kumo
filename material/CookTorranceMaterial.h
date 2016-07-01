//
// Created by 诸谦 on 16/1/19.
//

#ifndef RAYTRACER_COOKTORRANCEMATERIAL_H
#define RAYTRACER_COOKTORRANCEMATERIAL_H


#include "Material.h"
#include "../brdf/Lambertian.h"
#include "../brdf/CookTorrance.h"


class CookTorranceMaterial : public Material{
private:
OldLambertian mAmbientBrdf;
    CookTorrance mCookTorranceBrdf;

public:

void setSurfaceColor(const RGB& color,const double m);
void setAmbientColor(const RGB& color);

virtual RGB shade(ShadeRec &sr);

virtual RGB areaLightShade(ShadeRec &sr);
};


#endif //RAYTRACER_COOKTORRANCEMATERIAL_H
