//
// Created by 诸谦 on 15/12/29.
//

#ifndef RAYTRACER_TORRANCESPARROW_H
#define RAYTRACER_TORRANCESPARROW_H


#include "Material.h"
#include "../brdf/Lambertian.h"
#include "../brdf/TorranceSparrowSpecular.h"


class TorranceSparrow: public Material{
private:
    Lambertian mAmbientBrdf;
    Lambertian mDiffuseBrdf;
    TorranceSparrowSpecular mSpecularBrdf;

public:
    void scaleAmbientAlbedo(const float k);
    void scaleDiffuseAlbedo(const float k);
    void scaleSpecularAlbedo(const float k);
    void setAlbedo(const RGB& color,const double m);

    virtual RGB shade(ShadeRec &sr);

    virtual RGB areaLightShade(ShadeRec &sr);
};


#endif //RAYTRACER_TORRANCESPARROW_H
