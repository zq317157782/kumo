//
// Created by 诸谦 on 15/12/28.
//

#include "Shape.h"
#include "transform.h"

Shape::Shape(const Transform *o2w,const Transform *w2o,bool ro, bool mShadow)
        : localToWorld(o2w),worldToLocal(w2o),ReverseOrientation(ro),TransformSwapsHandedness(o2w->SwapsHandedness()), mShadow(mShadow),shapeID(nextShapeID++) { }
bool Shape::Intersect(const Ray& ray,float* distance,float *rayEpsilon, DifferentialGeometry *dg) const {return false;}

//bool Shape::castShadow() const{
//    return mShadow;
//}
//
//void Shape::setCastShadow(bool s) {
//    mShadow=s;
//}

unsigned int Shape::nextShapeID=0;
