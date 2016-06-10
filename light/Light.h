//
// Created by 诸谦 on 15/12/25.
//

#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H


#include "../base/Vector3.h"
#include "../base/RGB.h"
#include "../common/ShadeRec.h"

/**
 * 光 基类
 */
class Light {
private:
    bool mShadow;//是否投射阴影
public:
    virtual Vector3 getDirection(const ShadeRec& sr) const =0; //得到光线方向
    virtual RGB L(const ShadeRec& sr)=0;  //得到hit点从这个光源得到的能量
    bool castShadow() const;
    virtual bool inShadow(const Ray& ray,const ShadeRec& sr)const;

    virtual float G(const ShadeRec& sr) const;
    virtual float pdf(ShadeRec& sr) const;//概率分布函数
};


#endif //RAYTRACER_LIGHT_H
