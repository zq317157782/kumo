//
// Created by 诸谦 on 15/12/24.
//

#include "RGB.h"
#include <assert.h>
#include <algorithm>
using namespace std;


RGB::RGB(float _r, float _g,  float _b):r(_r),g(_g),b(_b){
   // assert(r>=0&&b>=0&&g>=0);
}

RGB::RGB(float _all):r(_all),g(_all),b(_all){
   // assert(r>=0&&b>=0&&g>=0);
}

RGB RGB::operator+(const RGB &vec) const {
    return RGB(r+vec.r,g+vec.g,b+vec.b);
}

RGB RGB::operator+(float f) const {
    return RGB(r+f,g+f,b+f);
}

RGB RGB::operator-(const RGB &vec) const {
    return RGB(r-vec.r,g-vec.g,b-vec.b);
}


RGB RGB::operator-(float d) const {
    return RGB(r-d,g-d,b-d);
}

RGB RGB::operator*(float v) const {
    return RGB(r*v,g*v,b*v);
}

RGB RGB::operator*(const RGB &c) const {
    return RGB(r*c.r,g*c.g,b*c.b);
}

RGB &RGB::operator+=(const RGB &c) {
    return *this=*this+c;
}

RGB &RGB::operator-=(const RGB &c) {
    return *this=*this-c;
}

RGB &RGB::operator*=(const RGB &c) {
    return *this=*this*c;;
}

RGB operator+(float a, const RGB &b) {
    return b+a;
}

RGB operator*(float a, const RGB &b) {
    return b*a;
}

bool RGB::operator==(const RGB &c) const {
    return r==c.r&&g==c.g&&b==c.b;
}

bool RGB::operator!=(const RGB &c) const {
    return !(*this==c);
}

RGB RGB::operator/(float div) const {
    return RGB(r/div,g/div,b/div);
}

RGB RGB::operator/(const RGB& c) const {
    return RGB(r/c.r,g/c.g,b/c.b);
}

RGB RGB::clamp(){
    return RGB(max(0.0f,min(1.0f,r)), max(0.0f, min(1.0f,g)), max(0.0f, min(1.0f,b)));
}
