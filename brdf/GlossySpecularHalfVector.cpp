//
// Created by 诸谦 on 15/12/29.
//

#include <math.h>
#include "GlossySpecularHalfVector.h"

RGB GlossySpecularHalfVector::f(const Intersection &sr, const Vector &wi, const Vector &wo) {

    Vector h=Normalize(wi + wo);//求得半向量
    double ndoth=Dot(sr.normal,h);
    RGB L(0,0,0);
    if(ndoth>0)
        L= mKs * mScaleFactor * pow(ndoth, mShiness);//计算高光
    return L;
}
