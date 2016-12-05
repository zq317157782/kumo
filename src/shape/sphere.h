//
// Created by 诸谦 on 15/12/28.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H


#include <shape.h>
#include <stdio.h>
class Sphere : public Shape {
private:
     Float mRad;//半径
    Float mPhiMax; //最大phi值  Y轴和X轴之间的角度
    Float mZMin, mZMax;  //最大和最小的z值
    Float mThetaMin, mThetaMax; //最大和最小的theta值  绕y轴的角度
public:
    Sphere(const Transform *o2w,const Transform *w2o,bool ro, Float rad,Float zmin, Float zmax, Float phiMax);

    virtual ~Sphere() { }

    virtual bool Intersect(const Ray &ray, Float *distance,Float *rayEpsilon,  DifferentialGeometry *dg) const override;

    virtual bool CanIntersect() const;

    virtual bool IntersectP(const Ray& ray) const override;

    virtual Float Area() const override;

    virtual Point Sample(Float u1, Float u2, Normal *Ns) const override;
    virtual BBox ObjectBound() const override{
    	 return BBox(Point(-mRad, -mRad, mZMin),
    	                Point( mRad,  mRad, mZMax));
    };
};


#endif //RAYTRACER_SPHERE_H
