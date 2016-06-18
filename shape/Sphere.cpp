//
// Created by 诸谦 on 15/12/28.
//


#include "Sphere.h"
#include "transform.h"

Vector Sphere::getNormal(const Point &point) const {
    return  Normalize(Vector((*worldToLocal)(point)));
}

bool Sphere::hit(const Ray &r, double &distance, ShadeRec &sr) {
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



    sr.material=mMaterial;//设置材质
    sr.normal=Normalize(Vector(phit));
    sr.distance=thit;
    distance=thit;
    return true;


//    Vector v= ray.o - this->mPosition;//圆心到射线原点的向量
//    double b=2*Dot(v,ray.d);
//    double c=Dot(v,v)-mRad*mRad;
//    double delta=b*b-4*c;
//    if(delta<0.0){
//        return false;
//    } else{
//        delta=sqrt(delta);
//        double t1=(-b-delta)/2;
//        if(t1>EPSILON){
//            sr.material=mMaterial;//设置材质
//            Point v=ray(t1);
//            sr.normal=Normalize(v-mPosition);
//            sr.distance=t1;
//            distance=t1;
//            return true;
//        }
//
//        double t2=(-b+delta)/2;
//        if(t2>EPSILON){
//            sr.material=mMaterial;//设置材质
//            Point v=ray(t2);
//            sr.normal=Normalize(v-mPosition);
//            sr.distance=t2;
//            distance=t2;
//            return true;
//        }
//        return false;
//    }
}

bool Sphere::shadowHit(const Ray &ray, double &distance) const{
//    Vector v= ray.o - this->mPosition;//圆心到射线原点的向量
//    double b=2*Dot(v,ray.d);
//    double c=Dot(v,v)-mRad*mRad;
//    double delta=b*b-4*c;
//    if(delta<0.0){
//        return false;
//    } else{
//        delta=sqrt(delta);
//        double t1=(-b-delta)/2;
//        if(t1>EPSILON){
//            distance=t1;
//            return true;
//        }
//
//        double t2=(-b+delta)/2;
//        if(t2>EPSILON){
//            distance=t2;
//            return true;
//        }
//        return false;
//    }
}

Sphere::Sphere(Transform *o2w,Transform *w2o, float rad, float z0, float z1, float phiMax,Material* mMaterial, bool mShadow): Shape(o2w,w2o,mMaterial, mShadow), mRad(rad){

    mZMin = Clamp(min(z0, z1), -mRad, mRad);
    mZMax = Clamp(max(z0, z1), -mRad, mRad);
    mThetaMin = acosf(Clamp(mZMin/mRad, -1.f, 1.f));
    mThetaMax = acosf(Clamp(mZMax/mRad, -1.f, 1.f));
    mPhiMax = Radians(Clamp(phiMax, 0.0f, 360.0f));
}
