//
// Created by 诸谦 on 15/12/25.
//

#ifndef RAYTRACER_LAMBERTIAN_H
#define RAYTRACER_LAMBERTIAN_H


#include "BRDF.h"

class Lambertian: public BRDF{
private:
    float mScaleFactor;
    RGB mAlbedo;
public:
    Lambertian(const RGB &_albedo=RGB(1,1,1), float _scaleFactor=1.0);
    void scaleAlbedo(float _f);
    void setAlbedo(const RGB& _albedo);


    virtual RGB f(const ShadeRec &sr, const Vector3 &wi, const Vector3 &wo) override;

    virtual RGB rho(const ShadeRec &sr, const Vector3 &wo) override;
};


#endif //RAYTRACER_LAMBERTIAN_H
