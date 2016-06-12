//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_DIRECTIONAL_H
#define RAYTRACER_DIRECTIONAL_H


#include "Light.h"

class Directional: public Light{
private:
    RGB mIrradiance;
    float mScaleFactor;
    Vector mDirection;

public:
    Directional(const RGB &mIrradiance=RGB(1,1,1), const Vector &mDirection=Vector(1,0,0),const float mScaleFactor=1.0);

    virtual Vector getDirection(const ShadeRec &sr) const override;

    virtual RGB L(const ShadeRec &sr)  override;


    virtual bool inShadow(const Ray &ray, const ShadeRec &sr) const override;
};


#endif //RAYTRACER_DIRECTIONAL_H
