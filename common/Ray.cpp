//
// Created by 诸谦 on 15/12/27.
//

#include "Ray.h"

Ray::Ray(const Vector3 &position, const Vector3 &direction) : position(position), direction(direction){
    this->direction.normalize();
}

Vector3 Ray::getPointByDistance(const float dist) const{
    return position+dist*direction;
}
