//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_PHONGMATERIAL_H
#define RAYTRACER_PHONGMATERIAL_H


#include "Material.h"
#include "../brdf/Lambertian.h"
#include "../brdf/GlossySpecular.h"
#include <memory>
using namespace std;

class PhongMaterial : public Material{
private:
    OldLambertian mAmbientBrdf;
    OldLambertian mDiffuseBrdf;
    GlossySpecular mSpecularBrdf;
public:
//    void scaleAmbientAlbedo(const float k);
//    void scaleDiffuseAlbedo(const float k);
//    void scaleSpecularAlbedo(const float k);
//    void setAlbedo(const RGB& color,const float exp);


    void setSurfaceColor(const RGB& color,const float shiness);
    void setDiffuseColor(const RGB& color);
    void setAmbientColor(const RGB& color);
    virtual RGB shade(ShadeRec &sr);

    virtual RGB areaLightShade(ShadeRec &sr);
};


#endif //RAYTRACER_PHONGMATERIAL_H
