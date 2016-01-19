//
// Created by 诸谦 on 15/12/29.
//

#ifndef RAYTRACER_BLINNPHONGMATERIAL_H
#define RAYTRACER_BLINNPHONGMATERIAL_H


#include "Material.h"
#include "../brdf/Lambertian.h"
#include "../brdf/GlossySpecularHalfVector.h"


class BlinnPhongMaterial : public Material{
private:
    Lambertian mAmbientBrdf;
    Lambertian mDiffuseBrdf;
    GlossySpecularHalfVector mSpecularBrdf;
public:

//    void scaleAmbientAlbedo(const float k);
//    void scaleDiffuseAlbedo(const float k);
//    void scaleSpecularAlbedo(const float k,const float exp);
    void setSurfaceColor(const RGB& color,const float shiness);
    void setDiffuseColor(const RGB& color);
    void setAmbientColor(const RGB& color);
    virtual RGB shade(ShadeRec &sr);

    virtual RGB areaLightShade(ShadeRec &sr);
};


#endif //RAYTRACER_BLINNPHONGMATERIAL_H
