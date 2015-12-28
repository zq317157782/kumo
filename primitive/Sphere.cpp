//
// Created by 诸谦 on 15/12/28.
//

#include <math.h>
#include "Sphere.h"


Vector3 Sphere::getNormal(const Vector3 &point) const {
    return (point-this->mPosition).normalize();
}

bool Sphere::hit(const Ray &ray, double &distance, ShadeRec &sr) {
    Vector3 v=ray.position-this->mPosition;//圆心到射线原点的向量
    double b=2*v.dot(ray.direction);
    double c=v.dot(v)-mRad*mRad;
    double delta=b*b-4*c;
    if(delta<0.0){
        return false;
    } else{
        delta=sqrt(delta);
        double t1=(-b-delta)/2;
        if(t1>EPSILON){
            sr.material=mMaterial;//设置材质
            Vector3 v=ray.getPointByDistance(t1);
            sr.normal=(v-mPosition).normalize();
            sr.distance=t1;
            distance=t1;
            return true;
        }

        double t2=(-b+delta)/2;
        if(t2>EPSILON){
            sr.material=mMaterial;//设置材质
            Vector3 v=ray.getPointByDistance(t2);
            sr.normal=(v-mPosition).normalize();
            sr.distance=t2;
            distance=t2;
            return true;
        }
        return false;
    }
}

bool Sphere::shadowHit(const Ray &ray, double &distance) const{
    Vector3 v=ray.position-this->mPosition;//圆心到射线原点的向量
    double b=2*v.dot(ray.direction);
    double c=v.dot(v)-mRad*mRad;
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

Sphere::Sphere(const Vector3 &mPosition, const double rad,Material* mMaterial, bool mShadow):Primitive(mPosition,mMaterial,mShadow),mRad(rad){

}
