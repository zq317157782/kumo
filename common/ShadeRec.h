//
// Created by 诸谦 on 15/12/25.
//

#ifndef RAYTRACER_SHADEREC_H
#define RAYTRACER_SHADEREC_H

#include "../base/Vector3.h"


class Scene;
class Ray;

class ShadeRec {
public:
    Vector3 normal;//法线
    Vector3 hitPoint;//射线交点
    Scene& scene; //当前场景
    Ray& ray; //当前射线
};


#endif //RAYTRACER_SHADEREC_H
