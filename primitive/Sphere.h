//
// Created by 诸谦 on 15/12/28.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H


#include "Primitive.h"
#include <stdio.h>
class Sphere : public Primitive{
private:
    double mRad;//半径
public:
    Sphere(const Point &mPosition= Point(0, 0, 0), const double rad=1, Material* mMaterial=NULL, bool mShadow=true);

    virtual ~Sphere() { printf("Sphere Destroy");}

    virtual Vector getNormal(const Point &point) const;

    virtual bool hit(const Ray &ray, double &distance, ShadeRec &sr);


    virtual bool shadowHit(const Ray &ray, double &distance) const;
};


#endif //RAYTRACER_SPHERE_H
