//
// Created by 诸谦 on 15/12/29.
//

#include <math.h>
#include "HalfVectorSpecular.h"

RGB HalfVectorSpecular::f(const ShadeRec &sr, const Vector3 &wi, const Vector3 &wo) {

    Vector3 h=(wi+wo).normalize();//求得半向量
    double ndoth=sr.normal.dot(h);
    RGB L(0,0,0);
    if(ndoth>0)
        L=mAlbedo*mScaleFactor*pow(ndoth,mExp);//计算高光
    return L;
}
