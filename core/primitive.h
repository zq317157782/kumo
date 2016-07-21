//
// Created by 诸谦 on 16/6/11.
//

#ifndef RAYTRACER_PRIMITIVE_H
#define RAYTRACER_PRIMITIVE_H
#include <Intersection.h>
#include <material.h>
#include "global.h"
#include "memory.h"
#include "Shape.h"

//图元
class Primitive: public ReferenceCounted {
protected:
	static unsigned int nextPrimitiveID;
public:
	const unsigned int primitiveID;
	Primitive() :
			primitiveID(nextPrimitiveID++) {
	}
	virtual ~Primitive() {
	}
	virtual bool CanIntersect() const {
		return false;
	}
	;
	virtual bool Intersect(const Ray &r, Intersection *in) const = 0;
	virtual bool IntersectP(const Ray& ray) const=0;
	virtual Reference<Material> GetMaterial() const {
		assert(false);
		return nullptr;
	}
	;
	virtual Reference<Shape> GetShape() const {
		assert(false);
		return nullptr;
	}

	virtual void Refine(vector<Reference<Primitive> > &refined) const {
		assert(false);
	}
	;
};

class GeomPrimitive: public Primitive {
private:
	Reference<Shape> mShape;
	Reference<Material> mMaterial;
public:
	GeomPrimitive(const Reference<Shape>& s, const Reference<Material>& m) :
			Primitive(), mShape(s), mMaterial(m) {

	}

	bool CanIntersect() const override {
		return mShape->CanIntersect();
	}

	bool Intersect(const Ray &r, Intersection *in) const {
		float thit, rayEpsilon;
		bool ret = mShape->Intersect(r, &thit, &rayEpsilon, &(in->dg));
		if (ret) {
			in->distance = thit;
			in->normal = Vector(in->dg.nn);
			in->primitiveID = primitiveID;
		}
		return ret;
	}

	bool IntersectP(const Ray &r) const {
		return mShape->IntersectP(r);
	}

	Reference<Material> GetMaterial() const override {
		return mMaterial;
	}
	Reference<Shape> GetShape() const override {
		return mShape;
	}

	virtual void Refine(vector<Reference<Primitive> > &refined) const {
		vector<Reference<Shape> > r;
		mShape->Refine(r);
		for (int i = 0; i < r.size(); ++i) {
			GeomPrimitive *gp = new GeomPrimitive(r[i], mMaterial);
			refined.push_back(gp);
		}
	}
};

#endif //RAYTRACER_PRIMITIVE_H
