//
// Created by 诸谦 on 15/12/25.
//

#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H



#include <Intersection.h>
#include "RGB.h"
#include "memory.h"

/**
 * 光 基类
 */
class Light:public ReferenceCounted{
private:
    bool mShadow;//是否投射阴影
public:
    virtual Vector getDirection(const Intersection& sr) const =0; //得到光线方向
    virtual RGB L(const Intersection& sr)=0;  //得到hit点从这个光源得到的能量
    bool castShadow() const;
    virtual bool inShadow(const Ray& ray,const Intersection& sr)const;

    virtual float G(const Intersection& sr) const;
    virtual float pdf(Intersection& sr) const;//概率分布函数

    virtual ~Light(){};
};


#endif //RAYTRACER_LIGHT_H
