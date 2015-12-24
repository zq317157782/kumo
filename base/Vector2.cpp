//
// Created by 诸谦 on 15/12/24.
//

#include <math.h>
#include "Vector2.h"


Vector2::Vector2(const double& _x,const double& _y):x(_x),y(_y){

}

Vector2::Vector2(const double& _all):x(_all),y(_all){

}


Vector2 Vector2::operator+(const Vector2& vec) const{
    return Vector2(x+vec.x,y+vec.y);
}

Vector2 Vector2::operator-(const Vector2& vec) const{
    return Vector2(x-vec.x,y-vec.y);
}

Vector2 Vector2::operator-() const{
    return Vector2(-x,-y);
}

Vector2 Vector2::operator*(double v) const{
    return Vector2(x*v,y*v);
}
Vector2 Vector2::operator*(const Vector2& vec) const{
    return Vector2(x*vec.x,y*vec.y);
}

Vector2& Vector2::operator+=(const Vector2& vec){
    return *this=*this+vec;
}

Vector2 &Vector2::operator-=(const Vector2 &vec) {
    return  *this=*this-vec;;
}

Vector2& Vector2::operator*=(const Vector2& vec){
    return *this=*this*vec;
}

double Vector2::modulo() const {
    return sqrt(x*x+y*y);
}

Vector2& Vector2::normalize(){
    return *this=unit();
}

Vector2 Vector2::unit() const{
    return Vector2(*this * (1/modulo()));
}

double Vector2::dot(const Vector2& vec) const{
    return x*vec.x+y*vec.y;
}

double Vector2::distance2(const Vector2& b) const{
    Vector2 c(x-b.x,y-b.y);
    return c.modulo();
}

Vector2 Vector2::operator+(const double& d) const {
    return Vector2(x+d, y+d);
}

Vector2 Vector2::operator-(const double &d) const {
    return Vector2(x-d, y-d);
}

Vector2 operator+(const double& a, const Vector2 &b){
    return b+a;
}

Vector2 operator*(const double &a,const Vector2 &b){
    return b*a;
}

bool Vector2::operator==(const Vector2 &vec) const {
    return x == vec.x && y == vec.y;
}

bool Vector2::operator!=(const Vector2 &vec) const {
    return !(*this==vec);
}
