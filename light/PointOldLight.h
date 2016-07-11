//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_POINTLIGHT_H
#define RAYTRACER_POINTLIGHT_H


#include <OldLight.h>
#include "global.h"
#include "geometry.h"

class PointLight: public OldLight{
private:
    Point mPosition;
    RGB mIrradiance;
    float mScaleFactor;

public:
    PointLight(const Point &mPosition=Point(0,0,0), const RGB &mIrradiance=RGB(1,1,1),const float mScaleFactor=1.0);

    virtual Vector getDirection(const Intersection &sr) const override;

    virtual RGB L(const Intersection &sr)  override;

    virtual bool inShadow(const Ray &ray, const Intersection &sr) const override;
};


#endif //RAYTRACER_POINTLIGHT_H
