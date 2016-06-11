//
// Created by 诸谦 on 15/12/28.
//

#include "Shape.h"

Shape::Shape(Transform *o2w,Transform *w2o, Material* mMaterial, bool mShadow)
        : localToWorld(o2w),worldToLocal(w2o), mMaterial(mMaterial), mShadow(mShadow) { }

void Shape::setMaterial(Material* material) {
    this->mMaterial=material;
}

bool Shape::castShadow() const{
    return mShadow;
}

void Shape::setCastShadow(bool s) {
    mShadow=s;
}
const double Shape::EPSILON=0.00001;
