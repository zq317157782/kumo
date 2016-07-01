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

    virtual Vector getDirection(const Intersection &sr) const override;

    virtual RGB L(const Intersection &sr)  override;


    virtual bool inShadow(const Ray &ray, const Intersection &sr) const override;
};


#endif //RAYTRACER_DIRECTIONAL_H
