/*
 * light.cpp
 *
 *  Created on: 2016年7月11日
 *      Author: Administrator
 */
#include "light.h"
#include "Scene.h"

Light::Light(const Transform& l2w,int ns):lightToWorld(l2w),worldToLight(Inverse(l2w)),numSamples(max(1, ns)){

}


bool VisibilityTester::Unoccluded(const Scene *scene) const {
    return !scene->IntersectP(r);
}


