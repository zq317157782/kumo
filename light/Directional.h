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
    Directional(const RGB &mIrradiance=RGB(1,1,1), const Vector3 &mDirection=Vector3(1,0,0),const float mScaleFactor=1.0);

    virtual Vector3 getDirection(const ShadeRec &sr) const override;

    virtual RGB L(const ShadeRec &sr)  override;


    virtual bool inShadow(const Ray &ray, const ShadeRec &sr) const;
};


#endif //RAYTRACER_DIRECTIONAL_H
