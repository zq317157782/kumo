//
// Created by 诸谦 on 15/12/28.
//

#include "Primitive.h"

Primitive::Primitive(const Point &position,Material* mMaterial, bool mShadow)
        : mPosition(position), mMaterial(mMaterial), mShadow(mShadow) { }

void Primitive::setMaterial(Material* material) {
    this->mMaterial=material;
}

bool Primitive::castShadow() const{
    return mShadow;
}

void Primitive::setCastShadow(bool s) {
    mShadow=s;
}
const double Primitive::EPSILON=0.00001;
