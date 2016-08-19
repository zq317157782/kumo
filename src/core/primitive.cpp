//
// Created by 诸谦 on 16/6/11.
//

#include "primitive.h"

unsigned int Primitive::nextPrimitiveID=0;


void Primitive::FullyRefine(vector<Reference<Primitive> > &refined) const{
	vector<Reference<Primitive>> todo;
	todo.push_back(const_cast<Primitive *>(this));
	while(todo.size()){//todo.size()>0
		//取出最后一个图元
		Reference<Primitive> prim = todo.back();
		todo.pop_back();
		if(prim->CanIntersect()){
			refined.push_back(prim);
		}
		else{
			prim->Refine(todo);
		}
	}
}

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
