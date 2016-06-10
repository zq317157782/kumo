//
// Created by 诸谦 on 15/12/25.
//

#ifndef RAYTRACER_SHADEREC_H
#define RAYTRACER_SHADEREC_H


#include "global.h"
#include "geometry.h"
#include <memory>
using namespace std;
class Scene;
class Material;

class ShadeRec {
public:
    Vector normal;//法线
    Point hitPoint;//射线交点
    const Scene& scene; //当前场景
    Ray ray; //当前射线
    Material* material; //当前材质
    double distance;
    bool hitAnObject;
    ShadeRec(const Scene&,const Ray& ray);
    ShadeRec(const ShadeRec&);
};


#endif //RAYTRACER_SHADEREC_H
