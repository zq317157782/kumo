//
// Created by 诸谦 on 15/12/25.
//

#ifndef RAYTRACER_INTERSECTION_H
#define RAYTRACER_INTERSECTION_H


#include "global.h"
#include "geometry.h"
#include "diffgeom.h"
#include "transform.h"
using namespace std;


class Intersection {
public:
    Vector normal;//法线
    const Scene& scene; //当前场景
    Ray ray; //当前射线
    Material* material; //当前材质
    const Primitive *primitive;//相交图元
    unsigned int  primitiveID;
    Transform WorldToObject, ObjectToWorld;
    float rayEpsilon;
    double distance;
    DifferentialGeometry dg; //微分几何
    Intersection(const Scene&,const Ray& ray);
    Intersection(const Intersection&);

    BSDF *GetBSDF(const RayDifferential &ray, MemoryArena &arena) const;
};


#endif //RAYTRACER_INTERSECTION_H
