//
// Created by 诸谦 on 15/12/25.
//

#ifndef RAYTRACER_LAMBERTIAN_H
#define RAYTRACER_LAMBERTIAN_H


#include "BRDF.h"

class OldLambertian: public BRDF{
private:
    float mScaleFactor;
    RGB mKd;
public:
    OldLambertian(const RGB &_albedo=RGB(1,1,1), float _scaleFactor=1.0);
    void scale(float _f);
    void setKd(const RGB &_kd);


    virtual RGB f(const ShadeRec &sr, const Vector &wi, const Vector &wo) override;

    virtual RGB rho(const ShadeRec &sr, const Vector &wo) override;
};


#endif //RAYTRACER_LAMBERTIAN_H
