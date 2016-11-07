/*
 * light.cpp
 *
 *  Created on: 2016年7月11日
 *      Author: Administrator
 */
#include <scene.h>
#include "light.h"
#include "sampler.h"

Light::Light(const Transform& l2w,int ns):lightToWorld(l2w),worldToLight(Inverse(l2w)),numSamples(max(1, ns)){

}

RGB Light::Le(const RayDifferential &r) const{
	return RGB(0);
}

bool VisibilityTester::Unoccluded(const Scene *scene) const {
    return !scene->IntersectP(r);
}


LightSampleOffsets::LightSampleOffsets(int count, Sample *sample) {
    nSamples = count;
    componentOffset = sample->Add1D(nSamples);
    posOffset = sample->Add2D(nSamples);
}


LightSample::LightSample(const Sample *sample,
        const LightSampleOffsets &offsets, uint32_t n) {
    uPos[0] = sample->twoD[offsets.posOffset][2*n];
    uPos[1] = sample->twoD[offsets.posOffset][2*n+1];
    uComponent = sample->oneD[offsets.componentOffset][n];
}

