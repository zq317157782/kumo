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

    sr.material=mMaterial;//设置材质
    Point v=ray(thit);
    sr.normal=Normalize(Vector(v));
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

Sphere::Sphere(Transform *o2w,Transform *w2o, float rad, Material* mMaterial, bool mShadow): Shape(o2w,w2o,mMaterial, mShadow), mRad(rad){

}
