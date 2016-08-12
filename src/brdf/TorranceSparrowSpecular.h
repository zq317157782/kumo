//
// Created by 诸谦 on 15/12/29.
//

#ifndef RAYTRACER_TORRANCESPARROWSPECULAR_H
#define RAYTRACER_TORRANCESPARROWSPECULAR_H

#include "BRDF.h"

/**
 * TorranceSparrowSpecular BRDF模型
 * 基于物理 微平面理论
 */
class TorranceSparrowSpecular: public BRDF{
private :
    RGB mAlbedo;//强度
    float mScaleFactor;
    double mM;//光滑因子
  //  double mF0;//入射光线在法线位置时候的光线反射强度

    double _NDF(const Vector & N, const Vector & H);//法线分布函数
    double _G(const Vector & N, const Vector & H, const Vector & wi, const Vector & wo);//几何衰减因子
    RGB _Fresnel(const Vector & wo, const Vector & H);
public:


    TorranceSparrowSpecular( const RGB &mAlbedo=RGB(1,1,1),double mM=1.0,float mScaleFactor=1.0);

    virtual RGB f(const Intersection &sr, const Vector &wi, const Vector &wo);

    virtual RGB rho(const Intersection &sr, const Vector &wo);

    void scaleAlbedo(float _f);
    void setAlbedo(const RGB& _albedo);
   // void setFresnelZero(const double f);//设置F0强度
    void setGlossy(const double m);//设置光滑度
};


#endif //RAYTRACER_TORRANCESPARROWSPECULAR_H
