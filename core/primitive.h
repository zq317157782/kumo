//
// Created by 诸谦 on 16/6/11.
//

#ifndef RAYTRACER_PRIMITIVE_H
#define RAYTRACER_PRIMITIVE_H
#include "global.h"
#include "memory.h"
#include "../common/ShadeRec.h"
#include "Material.h"
#include "Shape.h"

//图元
class Primitive:public ReferenceCounted{
protected:
    static unsigned int nextPrimitiveID;
public:
    const unsigned int primitiveID;
    Primitive():primitiveID(nextPrimitiveID++){}

    virtual bool CanIntersect() const {return false;};
    virtual bool Intersect(const Ray &r, ShadeRec *in) const = 0;
    virtual Reference<Material> GetMaterial() const {assert(false);return nullptr;};
};



class GeomPrimitive:public Primitive{
private:
	Reference<Shape> mShape;
	Reference<Material> mMaterial;
public:
	GeomPrimitive(const Reference<Shape>& s,const Reference<Material>& m):Primitive(),mShape(s),mMaterial(m){

	}

	bool CanIntersect() const override {return true;};

	bool Intersect(const Ray &r, ShadeRec *in) const{
		float thit, rayEpsilon;
		bool ret= mShape->hit(r,&thit,&rayEpsilon,&(in->dg),*in);
		if(ret){
			//in->material=mMaterial;
			in->distance=thit;
			in->normal=Vector(in->dg.nn);
		}
	}

	Reference<Material> GetMaterial() const override {return mMaterial;};
};

#endif //RAYTRACER_PRIMITIVE_H
