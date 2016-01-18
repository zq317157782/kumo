//
// Created by 诸谦 on 15/12/29.
//

#ifndef RAYTRACER_BLINNPHONG_H
#define RAYTRACER_BLINNPHONG_H


#include "Material.h"
#include "../brdf/Lambertian.h"
#include "../brdf/GlossySpecularHalfVector.h"


class BlinnPhong : public Material{
private:
    Lambertian mAmbientBrdf;
    Lambertian mDiffuseBrdf;
    GlossySpecularHalfVector mSpecularBrdf;
public:

    void scaleAmbientAlbedo(const float k);
    void scaleDiffuseAlbedo(const float k);
    void scaleSpecularAlbedo(const float k,const float exp);
    void setAlbedo(const RGB& color);

    virtual RGB shade(ShadeRec &sr);

    virtual RGB areaLightShade(ShadeRec &sr);
};


#endif //RAYTRACER_BLINNPHONG_H
