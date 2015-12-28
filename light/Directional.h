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
    Vector3 mDirection;

public:
    Directional(const RGB &mIrradiance, const Vector3 &mDirection,float mScaleFactor);

    virtual Vector3 getDirection(const ShadeRec &sr) const override;

    virtual RGB L(const ShadeRec &sr) const override;


    virtual bool inShadow(const Ray &ray, const ShadeRec &sr) const;
};


#endif //RAYTRACER_DIRECTIONAL_H
