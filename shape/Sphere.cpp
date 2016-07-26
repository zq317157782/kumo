//
// Created by 诸谦 on 15/12/28.
//


#include "Sphere.h"
#include "transform.h"
#include "diffgeom.h"

bool Sphere::Intersect(const Ray &r, float *distance,float *rayEpsilon, DifferentialGeometry *dg) const{
    Ray ray;
    (*worldToLocal)(r,&ray);
    // Compute quadratic sphere coefficients
    float A = ray.d.x*ray.d.x + ray.d.y*ray.d.y + ray.d.z*ray.d.z;
    float B = 2 * (ray.d.x*ray.o.x + ray.d.y*ray.o.y + ray.d.z*ray.o.z);
    float C = ray.o.x*ray.o.x + ray.o.y*ray.o.y +
              ray.o.z*ray.o.z - mRad*mRad;
    float t0, t1;
    if (!Quadratic(A, B, C, &t0, &t1))
        return false;

    // Compute intersection distance along ray
    if (t0 > ray.maxT || t1 < ray.minT)
        return false;
    float thit = t0;
    if (t0 < ray.minT) {
        thit = t1;
        if (thit > ray.maxT) return false;
    }


    //这里开始计算参数化变量


    //计算phi
    Point phit;
    float phi;
    phit = ray(thit);
    if (phit.x == 0.f && phit.y == 0.f) phit.x = 1e-5f * mRad; //排除除零的情况
    phi = atan2f(phit.y, phit.x);
    if (phi < 0.) phi += 2.f*M_PI; //保证phi在2PI之中


    //判断是否在Z坐标之间的裁剪空间中
    if ((mZMin > -mRad && phit.z < mZMin) ||
        (mZMax <  mRad && phit.z > mZMax) || phi > mPhiMax) {
        if (thit == t1) return false;
        if (t1 > ray.maxT) return false;
        thit = t1;

        phit = ray(thit);
        if (phit.x == 0.f && phit.y == 0.f) phit.x = 1e-5f * mRad;
        phi = atan2f(phit.y, phit.x);
        if (phi < 0.) phi += 2.f*M_PI;
        if ((mZMin > -mRad && phit.z < mZMin) ||
            (mZMax <  mRad && phit.z > mZMax) || phi > mPhiMax)
            return false;
    }



    // Find parametric representation of sphere hit
    //寻找参数化的u和v
    float u = phi / mPhiMax;
    float theta = acosf(Clamp(phit.z / mRad, -1.f, 1.f));
    float v = (theta - mThetaMin) / (mThetaMax - mThetaMin);



    // 计算偏导 偏导还不是很熟悉,所以这里照搬了PBRT的公式,详细公式可以查阅PBRT
    float zradius = sqrtf(phit.x*phit.x + phit.y*phit.y);
    float invzradius = 1.f / zradius;
    float cosphi = phit.x * invzradius;
    float sinphi = phit.y * invzradius;
    Vector dpdu(-mPhiMax * phit.y, mPhiMax * phit.x, 0);
    Vector dpdv = (mThetaMax-mThetaMin) *
                  Vector(phit.z * cosphi, phit.z * sinphi,
                         -mRad * sinf(theta));



    //计算法线的偏导
    Vector d2Pduu = -mPhiMax * mPhiMax * Vector(phit.x, phit.y, 0);
    Vector d2Pduv = (mThetaMax - mThetaMin) * phit.z * mPhiMax *
                    Vector(-sinphi, cosphi, 0.);
    Vector d2Pdvv = -(mThetaMax - mThetaMin) * (mThetaMax - mThetaMin) *
                    Vector(phit.x, phit.y, phit.z);


    float E = Dot(dpdu, dpdu);
    float F = Dot(dpdu, dpdv);
    float G = Dot(dpdv, dpdv);
    Vector N = Normalize(Cross(dpdu, dpdv));
    float e = Dot(N, d2Pduu);
    float f = Dot(N, d2Pduv);
    float g = Dot(N, d2Pdvv);


    float invEGF2 = 1.f / (E*G - F*F);
    Normal dndu = Normal((f*F - e*G) * invEGF2 * dpdu +
                         (e*F - f*E) * invEGF2 * dpdv);
    Normal dndv = Normal((g*F - f*G) * invEGF2 * dpdu +
                         (f*F - g*E) * invEGF2 * dpdv);

    const Transform &o2w = *localToWorld;

    *dg = DifferentialGeometry(o2w(phit), o2w(dpdu), o2w(dpdv),
                               o2w(dndu), o2w(dndv), u, v, this);
    *distance=thit;
    *rayEpsilon = 5e-4f * *distance; //交点处的float误差

    return true;
}

bool Sphere::CanIntersect() const{
	return true;
}

Sphere::Sphere(const Transform *o2w,const Transform *w2o, bool ro,float rad, float z0, float z1, float phiMax): Shape(o2w,w2o,ro), mRad(rad){

    mZMin = Clamp(min(z0, z1), -mRad, mRad);
    mZMax = Clamp(max(z0, z1), -mRad, mRad);
    mThetaMin = acosf(Clamp(mZMin/mRad, -1.f, 1.f));
    mThetaMax = acosf(Clamp(mZMax/mRad, -1.f, 1.f));
    mPhiMax = Radians(Clamp(phiMax, 0.0f, 360.0f));
}

//TODO 带参球体面积怎么算
float Sphere::Area() const{
	return mPhiMax*mRad*(mZMax-mZMin);
}
