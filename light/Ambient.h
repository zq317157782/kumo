//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_AMBIENT_H
#define RAYTRACER_AMBIENT_H

#include "Light.h"

/**
 * 环境光
 */
class Ambient:public Light{
private:
    RGB mIrradiance;//光 能量 入射光辐射度
    float mScaleFactor; //缩放因子

public:

    Ambient(const RGB &mIrradiance=RGB(0.2,0.2,0.2),const float mScaleFactor=1.0);

    virtual Vector3 getDirection(const ShadeRec &sr) const override;

    virtual RGB L(const ShadeRec &sr)  override;
};


#endif //RAYTRACER_AMBIENT_H
