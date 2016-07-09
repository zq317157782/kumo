//
// Created by 诸谦 on 15/12/28.
//

#ifndef RAYTRACER_SHAPE_H
#define RAYTRACER_SHAPE_H


#include <Intersection.h>
#include "global.h"
#include "memory.h"
#include "geometry.h"

using namespace std;

class Shape :public ReferenceCounted{
protected:
    static unsigned int nextShapeID;
public:
    const Transform * localToWorld;
    const Transform * worldToLocal;

    const unsigned int shapeID;

    const bool ReverseOrientation, TransformSwapsHandedness;
public:
    Shape(const Transform *o2w,const Transform *w2o,bool ro);
    //todo 从这个接口中去除Intersection参数
    /*判断与法线的碰撞*/
    virtual bool Intersect(const Ray& ray,float* distance,float *rayEpsilon, DifferentialGeometry *dg) const;
    virtual bool CanIntersect() const;
    virtual void Refine(vector<Reference<Shape> > &refined) const;//提炼函数  为一些几何体提炼更加合适的几何结构

    virtual ~Shape(){};
};


#endif //RAYTRACER_SHAPE_H
