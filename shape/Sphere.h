//
// Created by 诸谦 on 15/12/28.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H


#include "Shape.h"
#include <stdio.h>
class Sphere : public Shape {
private:
     float mRad;//半径
    float mPhiMax; //最大phi值  Y轴和X轴之间的角度
    float mZMin, mZMax;  //最大和最小的z值
    float mThetaMin, mThetaMax; //最大和最小的theta值  绕y轴的角度
public:
    Sphere(Transform *o2w,Transform *w2o, float rad,float zmin, float zmax, float phiMax, Material* mMaterial=NULL, bool mShadow=true);

    virtual ~Sphere() { printf("Sphere Destroy");}

    virtual Vector getNormal(const Point &point) const;

    virtual bool hit(const Ray &ray, float *distance, ShadeRec &sr) override;


    virtual bool shadowHit(const Ray &ray, double &distance) const;
};


#endif //RAYTRACER_SPHERE_H
