//
// Created by 诸谦 on 16/6/18.
//

#include "diffgeom.h"
#include "shape.h"

DifferentialGeometry::DifferentialGeometry(const Point &P,
                                           const Vector &DPDU, const Vector &DPDV,
                                           const Normal &DNDU, const Normal &DNDV,
                                           float uu, float vv, const Shape *sh)
        : p(P), dpdu(DPDU), dpdv(DPDV), dndu(DNDU), dndv(DNDV) {
    // Initialize _DifferentialGeometry_ from parameters
    nn = Normal(Normalize(Cross(dpdu, dpdv)));
    u = uu;
    v = vv;
    shape = sh;
    dudx = dvdx = dudy = dvdy = 0;

    // Adjust normal based on orientation and handedness
    if (shape && (shape->ReverseOrientation ^ shape->TransformSwapsHandedness))
        nn *= -1.f;
}