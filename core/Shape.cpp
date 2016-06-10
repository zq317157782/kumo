//
// Created by 诸谦 on 15/12/28.
//

#include "Shape.h"

Shape::Shape(const Point &position, Material* mMaterial, bool mShadow)
        : mPosition(position), mMaterial(mMaterial), mShadow(mShadow) { }

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
