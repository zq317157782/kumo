//
// Created by 诸谦 on 15/12/28.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H


#include "Primitive.h"

class Sphere : public Primitive{
private:
    double mRad;//半径
public:
    Sphere(const Vector3 &mPosition=Vector3(0,0,0),const double rad=1,Material* mMaterial=NULL, bool mShadow=true);

    virtual Vector3 getNormal(const Vector3 &point) const;

    virtual bool hit(const Ray &ray, double &distance, ShadeRec &sr);


    virtual bool shadowHit(const Ray &ray, double &distance) const;
};


#endif //RAYTRACER_SPHERE_H
