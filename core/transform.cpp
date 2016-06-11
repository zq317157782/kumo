//
// Created by 诸谦 on 16/6/9.
//

#include "global.h"
#include "transform.h"
#include "geometry.h"

Matrix4X4::Matrix4X4(float mm[4][4]) {
    memcpy(m,mm,16* sizeof(float));
}


Matrix4X4 Transpose(const Matrix4X4 &mm){
    return Matrix4X4(mm.m[0][0], mm.m[1][0], mm.m[2][0], mm.m[3][0],
                     mm.m[0][1], mm.m[1][1], mm.m[2][1], mm.m[3][1],
                     mm.m[0][2], mm.m[1][2], mm.m[2][2], mm.m[3][2],
                     mm.m[0][3], mm.m[1][3], mm.m[2][3], mm.m[3][3]);
}

//求逆矩阵 使用高斯-约旦法 代码取自PBRT
Matrix4X4 Inverse(const Matrix4X4 &mm) {
    int indxc[4], indxr[4];
    int ipiv[4] = { 0, 0, 0, 0 };
    float minv[4][4];
    memcpy(minv, mm.m, 4*4*sizeof(float));
    for (int i = 0; i < 4; i++) {
        int irow = -1, icol = -1;
        float big = 0.;
        // Choose pivot
        for (int j = 0; j < 4; j++) {
            if (ipiv[j] != 1) {
                for (int k = 0; k < 4; k++) {
                    if (ipiv[k] == 0) {
                        if (fabsf(minv[j][k]) >= big) {
                            big = float(fabsf(minv[j][k]));
                            irow = j;
                            icol = k;
                        }
                    }
                    else if (ipiv[k] > 1) throw "Singular matrix in MatrixInvert";

                    // Error("Singular matrix in MatrixInvert");
                }
            }
        }
        ++ipiv[icol];
        // Swap rows _irow_ and _icol_ for pivot
        if (irow != icol) {
            for (int k = 0; k < 4; ++k)
                swap(minv[irow][k], minv[icol][k]);
        }
        indxr[i] = irow;
        indxc[i] = icol;
        if (minv[icol][icol] == 0.)
            throw "Singular matrix in MatrixInvert";
            //Error("Singular matrix in MatrixInvert");

        // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
        float pivinv = 1.f / minv[icol][icol];
        minv[icol][icol] = 1.f;
        for (int j = 0; j < 4; j++)
            minv[icol][j] *= pivinv;

        // Subtract this row from others to zero out their columns
        for (int j = 0; j < 4; j++) {
            if (j != icol) {
                float save = minv[j][icol];
                minv[j][icol] = 0;
                for (int k = 0; k < 4; k++)
                    minv[j][k] -= minv[icol][k]*save;
            }
        }
    }
    // Swap columns to reflect permutation
    for (int j = 3; j >= 0; j--) {
        if (indxr[j] != indxc[j]) {
            for (int k = 0; k < 4; k++)
                swap(minv[k][indxr[j]], minv[k][indxc[j]]);
        }
    }
    return Matrix4X4(minv);
}



Vector Transform::operator()(const Vector &v) const{
   float x=v.x,y=v.y,z=v.z;
   return Vector(m.m[0][0]*x+m.m[0][1]*y+m.m[0][2]*z,
                 m.m[1][0]*x+m.m[1][1]*y+m.m[1][2]*z,
                 m.m[2][0]*x+m.m[2][1]*y+m.m[2][2]*z
   );
}

void Transform::operator()(const Vector &v, Vector *rv) const{
    float x=v.x,y=v.y,z=v.z;
    rv->x=m.m[0][0]*x+m.m[0][1]*y+m.m[0][2]*z;
    rv->y=m.m[1][0]*x+m.m[1][1]*y+m.m[1][2]*z;
    rv->z=m.m[2][0]*x+m.m[2][1]*y+m.m[2][2]*z;
}

Point Transform::operator()(const Point &p) const{
    float x=p.x,y=p.y,z=p.z;
    float xp=m.m[0][0]*x+m.m[0][1]*y+m.m[0][2]*z+m.m[0][3];
    float yp=m.m[1][0]*x+m.m[1][1]*y+m.m[1][2]*z+m.m[1][3];
    float zp=m.m[2][0]*x+m.m[2][1]*y+m.m[2][2]*z+m.m[2][3];
    float wp=m.m[3][0]*x+m.m[3][1]*y+m.m[3][2]*z+m.m[3][3];
    assert(wp!=0.0f);
    if(wp==1.) return Point(xp,yp,zp);
    else return Point(xp,yp,zp)/wp;
}

void Transform::operator()(const Point &p, Point *rp) const {
    float x=p.x,y=p.y,z=p.z;
    rp->x=m.m[0][0]*x+m.m[0][1]*y+m.m[0][2]*z+m.m[0][3];
    rp->y=m.m[1][0]*x+m.m[1][1]*y+m.m[1][2]*z+m.m[1][3];
    rp->z=m.m[2][0]*x+m.m[2][1]*y+m.m[2][2]*z+m.m[2][3];
    float wp=m.m[3][0]*x+m.m[3][1]*y+m.m[3][2]*z+m.m[3][3];
    assert(wp!=0.0f);
    if(wp!=1.) *rp/=wp;
}

Ray Transform::operator()(const Ray &r) const {
    Ray ray=r;
    (*this)(ray.o,&ray.o);
    (*this)(ray.d,&ray.d);
    return ray;
}

void Transform::operator()(const Ray &r, Ray *rr) const {
    (*this)(r.o,&rr->o);
    (*this)(r.d,&rr->d);
    if(&r!=rr){
        rr->minT=r.minT;
        rr->maxT=r.maxT;
        rr->depth=r.depth;
    }
}


bool Transform::SwapsHandedness() const {
    float det = ((m.m[0][0] *
                  (m.m[1][1] * m.m[2][2] -
                   m.m[1][2] * m.m[2][1])) -
                 (m.m[0][1] *
                  (m.m[1][0] * m.m[2][2] -
                   m.m[1][2] * m.m[2][0])) +
                 (m.m[0][2] *
                  (m.m[1][0] * m.m[2][1] -
                   m.m[1][1] * m.m[2][0])));
    return det < 0.f;
}

//位移转换 参考PBRT
Transform Translate(const Vector &delta) {
    Matrix4X4 m(1, 0, 0, delta.x,
                0, 1, 0, delta.y,
                0, 0, 1, delta.z,
                0, 0, 0,       1);
    Matrix4X4 minv(1, 0, 0, -delta.x,
                   0, 1, 0, -delta.y,
                   0, 0, 1, -delta.z,
                   0, 0, 0,        1);
    return Transform(m, minv);
}
