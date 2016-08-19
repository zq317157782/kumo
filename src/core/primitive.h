//
// Created by 诸谦 on 16/6/11.
//

#ifndef RAYTRACER_PRIMITIVE_H
#define RAYTRACER_PRIMITIVE_H
#include <Intersection.h>
#include <material.h>
#include <shape.h>
#include "global.h"
#include "memory.h"

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
		cerr<<"undifine"<<endl;
		assert(false);
		return nullptr;
	}
	;
	virtual Reference<Shape> GetShape() const {
		cerr<<"undifine"<<endl;
		assert(false);
		return nullptr;
	}

	virtual void Refine(vector<Reference<Primitive> > &refined) const {
		cerr<<"undifine"<<endl;
		assert(false);
	}
	void FullyRefine(vector<Reference<Primitive> > &refined) const;

	virtual BSDF *GetBSDF(const DifferentialGeometry &dg,
			const Transform &ObjectToWorld, MemoryArena &arena) const = 0;

	virtual const AreaLight* GetAreaLight() const=0;
};

class GeomPrimitive: public Primitive {
private:
	Reference<Shape> mShape;
	Reference<Material> mMaterial;
	AreaLight* mAreaLight;
public:
	GeomPrimitive(const Reference<Shape>& s, const Reference<Material>& m,
			AreaLight * area = nullptr) :
			Primitive(), mShape(s), mMaterial(m), mAreaLight(area) {

	}

	bool CanIntersect() const override {
		return mShape->CanIntersect();
	}

	bool Intersect(const Ray &r, Intersection *in) const {
		float thit, rayEpsilon;
		bool ret = mShape->Intersect(r, &thit, &rayEpsilon, &(in->dg));
		if (ret) {
			in->primitive = this;
			in->distance = thit;
			in->normal = Vector(in->dg.nn);
			in->primitiveID = primitiveID;
			in->ObjectToWorld = *mShape->localToWorld;
			in->WorldToObject = *mShape->worldToLocal;
			in->rayEpsilon = rayEpsilon;
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

	virtual BSDF *GetBSDF(const DifferentialGeometry &dg,
			const Transform &ObjectToWorld, MemoryArena &arena) const override;

	virtual const AreaLight* GetAreaLight() const override;
};

//代表图元集合
class Aggregate: public Primitive {
	const AreaLight *GetAreaLight() const;
	BSDF *GetBSDF(const DifferentialGeometry &dg, const Transform &,
			MemoryArena &) const;
};

#endif //RAYTRACER_PRIMITIVE_H
