//
// Created by 诸谦 on 15/12/28.
//

#include "Shape.h"
#include "transform.h"

Shape::Shape(const Transform *o2w,const Transform *w2o,bool ro)
        : localToWorld(o2w),worldToLocal(w2o),ReverseOrientation(ro),TransformSwapsHandedness(o2w->SwapsHandedness()),shapeID(nextShapeID++) { }
bool Shape::Intersect(const Ray& ray,float* distance,float *rayEpsilon, DifferentialGeometry *dg) const {return false;}
bool Shape::IntersectP(const Ray& ray) const {return false;};
bool Shape::CanIntersect() const{assert(false);return false;}

void  Shape::Refine(vector<Reference<Shape> > &refined) const{
	assert(false);
}

float Shape::Area() const{
	assert(false);
}

unsigned int Shape::nextShapeID=0;
