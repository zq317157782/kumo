//
// Created by 诸谦 on 15/12/28.
//

#include "Shape.h"
#include "transform.h"

Shape::Shape(Transform *o2w,Transform *w2o,bool ro, Material* mMaterial, bool mShadow)
        : localToWorld(o2w),worldToLocal(w2o),ReverseOrientation(ro),TransformSwapsHandedness(o2w->SwapsHandedness()),mMaterial(mMaterial), mShadow(mShadow),shapeID(nextShapeID++) { }

void Shape::setMaterial(Material* material) {
    this->mMaterial=material;
}

bool Shape::castShadow() const{
    return mShadow;
}

void Shape::setCastShadow(bool s) {
    mShadow=s;
}

unsigned int Shape::nextShapeID=0;