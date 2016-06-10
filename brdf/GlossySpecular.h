//
// Created by 诸谦 on 15/12/25.
//

#ifndef RAYTRACER_GLOSSYSPECULAR_H
#define RAYTRACER_GLOSSYSPECULAR_H


#include "BRDF.h"

class GlossySpecular: public BRDF{
protected:
    RGB mKs;
    float mShiness;
    float mScaleFactor;
public:
    GlossySpecular(const RGB &_albedo=RGB(1,1,1),float _exp=1,float _scaleFactor=1.0);
    void scale(float _f);
    void setKs(const RGB &_ks);
    void setShiness(float _shiness);
    virtual RGB f(const ShadeRec &sr, const Vector &wi, const Vector &wo) override;
    virtual RGB rho(const ShadeRec &sr, const Vector &wo) override;

};


#endif //RAYTRACER_GLOSSYSPECULAR_H
