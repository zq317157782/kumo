//
// Created by 诸谦 on 15/12/25.
//

#ifndef RAYTRACER_INTERSECTION_H
#define RAYTRACER_INTERSECTION_H


#include <kumo.h>
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
    Float rayEpsilon;
    double distance;
    DifferentialGeometry dg; //微分几何
    BSDF *GetBSDF(const RayDifferential &ray, MemoryArena &arena) const;
    RGB Le(const Vector &wo) const; //返回相交点的自发光辐射率
};


#endif //RAYTRACER_INTERSECTION_H
