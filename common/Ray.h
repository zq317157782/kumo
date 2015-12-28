//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H


#include "../base/Vector3.h"

class Ray {
public:
    Vector3 position;//射线位置
    Vector3 direction;//射线方向

    Ray(const Vector3 &position=Vector3(0,0,0), const Vector3 &direction=Vector3(0,0,0));
    Vector3 getPointByDistance(const float dist) const;
};


#endif //RAYTRACER_RAY_H
