//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_POINTLIGHT_H
#define RAYTRACER_POINTLIGHT_H


#include "Light.h"
#include "global.h"
#include "geometry.h"

class PointLight: public Light{
private:
    Point mPosition;
    RGB mIrradiance;
    float mScaleFactor;

public:
    PointLight(const Point &mPosition=Point(0,0,0), const RGB &mIrradiance=RGB(1,1,1),const float mScaleFactor=1.0);

    virtual Vector getDirection(const ShadeRec &sr) const override;

    virtual RGB L(const ShadeRec &sr)  override;

    virtual bool inShadow(const Ray &ray, const ShadeRec &sr) const override;
};


#endif //RAYTRACER_POINTLIGHT_H
