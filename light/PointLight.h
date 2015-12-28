//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_POINTLIGHT_H
#define RAYTRACER_POINTLIGHT_H


#include "Light.h"

class PointLight: public Light{
private:
    Vector3 mPosition;
    RGB mIrradiance;
    float mScaleFactor;

public:
    PointLight(const Vector3 &mPosition=Vector3(0,0,0), const RGB &mIrradiance=RGB(1,1,1), float mScaleFactor=1.0);

    virtual Vector3 getDirection(const ShadeRec &sr) const override;

    virtual RGB L(const ShadeRec &sr) const override;

    virtual bool inShadow(const Ray &ray, const ShadeRec &sr) const;
};


#endif //RAYTRACER_POINTLIGHT_H
