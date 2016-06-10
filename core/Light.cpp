//
// Created by 诸谦 on 15/12/25.
//

#include "Light.h"

bool Light::castShadow() const{
 return mShadow;
}

bool Light::inShadow(const Ray &ray, const ShadeRec &sr) const {
 return false;
}

float Light::G(const ShadeRec &sr) const {
 return 1.0;
}

float Light::pdf(ShadeRec &sr) const {
 return 1.0;
}
