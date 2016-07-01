//
// Created by 诸谦 on 15/12/25.
//

#include <Intersection.h>
#include "global.h"

Intersection::Intersection(const Intersection& _sr):normal(_sr.normal),scene(_sr.scene),ray(_sr.ray),material(_sr.material),distance(_sr.distance){

}

/**
 *  Vector3 normal;//法线
    Vector3 hitPoint;//射线交点
    Scene& scene; //当前场景
    Ray& ray; //当前射线
    shared_ptr<Material> material; //当前材质
    double distance;
 */
Intersection::Intersection(const Scene& scene,const Ray& r): ray(r), normal(Vector(0, 0, 0)), scene(scene), material(NULL), distance(0){

}

