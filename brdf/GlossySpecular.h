//
// Created by 诸谦 on 15/12/25.
//

#ifndef RAYTRACER_GLOSSYSPECULAR_H
#define RAYTRACER_GLOSSYSPECULAR_H


#include "BRDF.h"

class GlossySpecular: public BRDF{
protected:
    RGB mAlbedo;
    float mExp;
    float mScaleFactor;
public:
    GlossySpecular(const RGB &_albedo=RGB(1,1,1),float _exp=1,float _scaleFactor=1.0);
    void scaleAlbedo(float _f);
    void setAlbedo(const RGB& _albedo);
    void setExponent(float exp);


    virtual RGB f(const ShadeRec &sr, const Vector3 &wi, const Vector3 &wo) override;

    virtual RGB rho(const ShadeRec &sr, const Vector3 &wo) override;

};


#endif //RAYTRACER_GLOSSYSPECULAR_H
