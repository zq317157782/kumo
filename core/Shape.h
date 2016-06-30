//
// Created by 诸谦 on 15/12/28.
//

#ifndef RAYTRACER_SHAPE_H
#define RAYTRACER_SHAPE_H


#include "global.h"
#include "memory.h"
#include "geometry.h"
#include "../common/ShadeRec.h"
using namespace std;

class Shape :public ReferenceCounted{
protected:
    static unsigned int nextShapeID;
public:
    const Transform * localToWorld;
    const Transform * worldToLocal;
    bool mShadow;//是否投射阴影

    const unsigned int shapeID;

    const bool ReverseOrientation, TransformSwapsHandedness;
public:
    Shape(Transform *o2w,Transform *w2o,bool ro,bool mShadow=true) ;

    /*判断与法线的碰撞*/
    virtual bool hit(const Ray& ray,float* distance,float *rayEpsilon, DifferentialGeometry *dg,ShadeRec& sr) const=0;
    virtual bool shadowHit(const Ray& ray,double& distance) const=0;

    bool castShadow() const;
    void setCastShadow(bool s);

    virtual ~Shape(){};
};


#endif //RAYTRACER_SHAPE_H
