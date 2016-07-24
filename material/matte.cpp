//
// Created by 诸谦 on 15/12/27.
//

#include <Scene.h>
#include "matte.h"

Matte::Matte(const Reference<Texture<RGB>>& _albedo):mR(_albedo){

}

//
//RGB Matte::shade(const Intersection &sr){
//	Vector wo=-sr.ray.d;
//    Point hitPoint=sr.ray(sr.distance);
//    Vector wi;
//    float pdf;
//    VisibilityTester vt;
//    RGB L(0,0,0);
//    LightSample ls;
//    unsigned long lignt_num=sr.scene.getLightNum();
//    for(unsigned long i=0;i<lignt_num;++i){
//        RGB l=sr.scene.getLight(i)->Sample_L(hitPoint,0,ls,&wi,&pdf,&vt);
//        if(!vt.Unoccluded(&sr.scene)) continue;
//        double ndotl=Dot(sr.normal,wi);
//        if(ndotl>0){
//            L=L+l*mDiffuseBrdf.f(wi,wo)*ndotl;  //mDiffuseBrdf.f(sr,wi,wo)
//        }
//    }
//    return L;
//}


BSDF *Matte::GetBSDF(const DifferentialGeometry &dgGeom,
                             const DifferentialGeometry &dgShading,
                             MemoryArena &arena) const{
	BSDF *bsdf=BSDF_ALLOC(arena,BSDF)(dgShading,dgGeom.nn);
	bsdf->Add(BSDF_ALLOC(arena,Lambertian)(mR->Evaluate(dgGeom)));
	return bsdf;
}
