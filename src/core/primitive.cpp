//
// Created by 诸谦 on 16/6/11.
//

#include "primitive.h"

unsigned int Primitive::nextPrimitiveID=0;


BSDF *GeomPrimitive::GetBSDF(const DifferentialGeometry &dg,
	        const Transform &ObjectToWorld, MemoryArena &arena) const{
	DifferentialGeometry shadingG;
	mShape->GetShadingGeometry(ObjectToWorld,dg,&shadingG);
	return mMaterial->GetBSDF(dg,shadingG,arena);
}



const AreaLight* GeomPrimitive::GetAreaLight() const{
	return mAreaLight;
}


const AreaLight *Aggregate::GetAreaLight() const {
    cerr<<"Aggregate::GetAreaLight() method"
         "called; should have gone to GeometricPrimitive"<<endl;
    return nullptr;
}


BSDF *Aggregate::GetBSDF(const DifferentialGeometry &,
        const Transform &, MemoryArena &) const {
	cerr<<"Aggregate::GetBSDF() method"
        "called; should have gone to GeometricPrimitive"<<endl;
    return nullptr;
}
