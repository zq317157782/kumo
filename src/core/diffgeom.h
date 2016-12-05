//
// Created by 诸谦 on 16/6/18.
//

#ifndef RAYTRACER_DIFFGEOM_H
#define RAYTRACER_DIFFGEOM_H
#include <kumo.h>
#include "geometry.h"

//微分几何
struct DifferentialGeometry{
    DifferentialGeometry() {
        u = v = dudx = dvdx = dudy = dvdy = 0.;
        shape = nullptr;
    }
    // DifferentialGeometry Public Methods
    DifferentialGeometry(const Point3f &P, const Vector3f &DPDU,
                         const Vector3f &DPDV, const Normal3f &DNDU,
                         const Normal3f &DNDV, Float uu, Float vv,
                         const Shape *sh);

    void ComputeDifferentials(const RayDifferential &r) const; //通过微分射线  计算  几何微分

    // DifferentialGeometry Public Data
    Point3f p;
    Normal3f nn;
    Float u, v;
    const Shape *shape;
    Vector3f dpdu, dpdv;
    Normal3f dndu, dndv;
    mutable Vector3f dpdx, dpdy;
    mutable Float dudx, dvdx, dudy, dvdy;
};

#endif //RAYTRACER_DIFFGEOM_H
