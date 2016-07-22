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
    Material* material; //当前材质
    const Primitive *primitive;//相交图元
    unsigned int  primitiveID;
    Transform WorldToObject, ObjectToWorld;
    float rayEpsilon;
    double distance;
    DifferentialGeometry dg; //微分几何
    BSDF *GetBSDF(const RayDifferential &ray, MemoryArena &arena) const;
};


#endif //RAYTRACER_INTERSECTION_H
