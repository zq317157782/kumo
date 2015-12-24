//
// Created by 诸谦 on 15/12/23.
//

#include <math.h>
#include "Vector3.h"


Vector3::Vector3(const double& _x,const double& _y,const double& _z):x(_x),y(_y),z(_z){

}

Vector3::Vector3(const double& _all):x(_all),y(_all),z(_all){

}


Vector3 Vector3::operator+(const Vector3& vec) const{
    return Vector3(x+vec.x,y+vec.y,z+vec.z);
}

Vector3 Vector3::operator-(const Vector3& vec) const{
    return Vector3(x-vec.x,y-vec.y,z-vec.z);
}

Vector3 Vector3::operator-() const{
    return Vector3(-x,-y,-z);
}

Vector3 Vector3::operator*(double v) const{
    return Vector3(x*v,y*v,z*v);
}
Vector3 Vector3::operator*(const Vector3& vec) const{
    return Vector3(x*vec.x,y*vec.y,z*vec.z);
}

Vector3& Vector3::operator+=(const Vector3& vec){
    return *this=*this+vec;
}

Vector3 &Vector3::operator-=(const Vector3 &vec) {
    return  *this=*this-vec;;
}

Vector3& Vector3::operator*=(const Vector3& vec){
    return *this=*this*vec;
}

double Vector3::modulo() const {
    return sqrt(x*x+y*y+z*z);
}

Vector3& Vector3::normalize(){
    return *this=unit();
}

Vector3 Vector3::unit() const{
    return Vector3(*this * (1/modulo()));
}

double Vector3::dot(const Vector3& vec) const{
    return x*vec.x+y*vec.y+z*vec.z;
}

Vector3 Vector3::cross(const Vector3& vec) const{
    return Vector3(y*vec.z-vec.y*z,z*vec.x-vec.z*x,x*vec.y-y*vec.x);
}

double Vector3::distance2(const Vector3& b) const{
    Vector3 c(x-b.x,y-b.y,z-b.z);
    return c.modulo();
}

Vector3 Vector3::operator+(const double& d) const {
    return Vector3(x+d, y+d, z+d);
}

Vector3 Vector3::operator-(const double &d) const {
    return Vector3(x-d, y-d, z-d);
}

Vector3 operator+(const double& a, const Vector3 &b){
    return b+a;
}

Vector3 operator*(const double &a,const Vector3 &b){
    return b*a;
}

bool Vector3::operator==(const Vector3 &vec) const {
    return x == vec.x && y == vec.y && z == vec.z;
}

bool Vector3::operator!=(const Vector3 &vec) const {
    return !(*this==vec);
}
