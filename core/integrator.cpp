/*
 * integrator.cpp
 *
 *  Created on: 2016年7月30日
 *      Author: zhuqian
 */
#include "integrator.h"
#include "global.h"
#include "light.h"
#include "Scene.h"
#include "montecarlo.h"
Distribution1D *ComputeLightSamplingCDF(const Scene *scene) {
    unsigned int nLights = scene->getLightNum();
    vector<float>lightPower(nLights, 0.0f);
    for (unsigned int i = 0; i < nLights; ++i)
        lightPower[i] = scene->getLight(i)->Power(scene).y();
    return new Distribution1D(&lightPower[0], nLights);
}



