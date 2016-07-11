//
// Created by 诸谦 on 15/12/25.
//

#include <OldLight.h>

bool OldLight::inShadow(const Ray &ray, const Intersection &sr) const {
 return false;
}

float OldLight::G(const Intersection &sr) const {
 return 1.0;
}

float OldLight::pdf(Intersection &sr) const {
 return 1.0;
}


bool OldLight::castShadow() const{return true;};
