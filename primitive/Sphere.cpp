//
// Created by 诸谦 on 15/12/28.
//


#include "Sphere.h"


Vector Sphere::getNormal(const Point &point) const {
    return Vector(point-this->mPosition);
}

bool Sphere::hit(const Ray &ray, double &distance, ShadeRec &sr) {
    Vector v= ray.o - this->mPosition;//圆心到射线原点的向量
    double b=2*Dot(v,ray.d);
    double c=Dot(v,v)-mRad*mRad;
    double delta=b*b-4*c;
    if(delta<0.0){
        return false;
    } else{
        delta=sqrt(delta);
        double t1=(-b-delta)/2;
        if(t1>EPSILON){
            sr.material=mMaterial;//设置材质
            Point v=ray(t1);
            sr.normal=Normalize(v-mPosition);
            sr.distance=t1;
            distance=t1;
            return true;
        }

        double t2=(-b+delta)/2;
        if(t2>EPSILON){
            sr.material=mMaterial;//设置材质
            Point v=ray(t2);
            sr.normal=Normalize(v-mPosition);
            sr.distance=t2;
            distance=t2;
            return true;
        }
        return false;
    }
}

bool Sphere::shadowHit(const Ray &ray, double &distance) const{
    Vector v= ray.o - this->mPosition;//圆心到射线原点的向量
    double b=2*Dot(v,ray.d);
    double c=Dot(v,v)-mRad*mRad;
    double delta=b*b-4*c;
    if(delta<0.0){
        return false;
    } else{
        delta=sqrt(delta);
        double t1=(-b-delta)/2;
        if(t1>EPSILON){
            distance=t1;
            return true;
        }

        double t2=(-b+delta)/2;
        if(t2>EPSILON){
            distance=t2;
            return true;
        }
        return false;
    }
}

Sphere::Sphere(const Point &position, const double rad, Material* mMaterial, bool mShadow): Shape(position, mMaterial, mShadow), mRad(rad){

}
