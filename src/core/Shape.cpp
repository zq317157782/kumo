//
// Created by 诸谦 on 15/12/28.
//

#include "Shape.h"
#include "transform.h"
#include "geometry.h"

Shape::Shape(const Transform *o2w, const Transform *w2o, bool ro) :
		localToWorld(o2w), worldToLocal(w2o), ReverseOrientation(ro), TransformSwapsHandedness(
				o2w->SwapsHandedness()), shapeID(nextShapeID++) {
}
bool Shape::Intersect(const Ray& ray, float* distance, float *rayEpsilon,
		DifferentialGeometry *dg) const {
	return false;
}
bool Shape::IntersectP(const Ray& ray) const {
	return false;
}
;
bool Shape::CanIntersect() const {
	assert(false);
	return false;
}

void Shape::Refine(vector<Reference<Shape> > &refined) const {
	assert(false);
}

float Shape::Area() const {
	assert(false);
	return 0;
}

float Shape::Pdf(const Point &p, const Vector& wi) const {
	DifferentialGeometry dgLight; //光源交点处的微分结构
	Ray ray(p, wi, 1e-3f); //射向光源的射线
	float distance = 0;
	float rayEpsilon = 0;
	if (!Intersect(ray, &distance, &rayEpsilon, &dgLight))
		return 0;
	//下面是area的pdf转换成空间角的pdf
	float pdf = DistanceSqr(p, ray(distance))
			/ (AbsDot(dgLight.nn, -wi) * Area());
	if (isinf(pdf)) pdf = 0.0f;
	return pdf;

}

BBox Shape::WorldBound() const {
          return (*localToWorld)(ObjectBound());
}

unsigned int Shape::nextShapeID = 0;
