//
// Created by 诸谦 on 15/12/28.
//

#ifndef RAYTRACER_PRIMITIVE_H
#define RAYTRACER_PRIMITIVE_H


#include "global.h"
#include "geometry.h"
#include "memory.h"
#include "Material.h"
#include "../material/Matte.h"
#include <memory>
using namespace std;

class Shape :public ReferenceCounted{
protected:
    static unsigned int nextShapeID;
public:
    const Transform * localToWorld;
    const Transform * worldToLocal;
    Material* mMaterial;//图元使用的材质
    bool mShadow;//是否投射阴影

    const unsigned int shapeID;

    const bool ReverseOrientation, TransformSwapsHandedness;
public:
    Shape(Transform *o2w,Transform *w2o,bool ro,Material *mMaterial=NULL, bool mShadow=true) ;

    void setMaterial(Material* material);
//    /*返回交点处的法线 要求标准化后*/
//    virtual Vector getNormal(const Point & point) const=0;

    /*判断与法线的碰撞*/
    virtual bool hit(const Ray& ray,float* distance,float *rayEpsilon, DifferentialGeometry *dg,ShadeRec& sr)=0;
    virtual bool shadowHit(const Ray& ray,double& distance) const=0;

    bool castShadow() const;
    void setCastShadow(bool s);

    virtual ~Shape(){};
};


#endif //RAYTRACER_PRIMITIVE_H
