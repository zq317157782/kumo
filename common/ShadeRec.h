//
// Created by 诸谦 on 15/12/25.
//

#ifndef RAYTRACER_SHADEREC_H
#define RAYTRACER_SHADEREC_H

#include "../base/Vector3.h"
#include "Ray.h"
#include <memory>
using namespace std;
class Scene;
class Material;

class ShadeRec {
public:
    Vector3 normal;//法线
    Vector3 hitPoint;//射线交点
    const Scene& scene; //当前场景
    Ray ray; //当前射线
    Material* material; //当前材质
    double distance;
    bool hitAnObject;
    ShadeRec(const Scene&);
    ShadeRec(const ShadeRec&);
};


#endif //RAYTRACER_SHADEREC_H
