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
    Sphere(const Transform *o2w,const Transform *w2o,bool ro, float rad,float zmin, float zmax, float phiMax);

    virtual ~Sphere() { }

    virtual bool Intersect(const Ray &ray, float *distance,float *rayEpsilon,  DifferentialGeometry *dg) const override;

    virtual bool CanIntersect() const;

    virtual bool IntersectP(const Ray& ray) const override;

    virtual float Area() const override;

    virtual Point Sample(float u1, float u2, Normal *Ns) const override;
    virtual BBox ObjectBound() const override{
    	 return BBox(Point(-mRad, -mRad, mZMin),
    	                Point( mRad,  mRad, mZMax));
    };
};


#endif //RAYTRACER_SPHERE_H
