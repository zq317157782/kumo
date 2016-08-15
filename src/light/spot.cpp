/*
 * spot.cpp
 *
 *  Created on: 2016年7月12日
 *      Author: Administrator
 */
#include "spot.h"

SpotLight::SpotLight(const Transform& l2w, const RGB& intensity, float width,
		float fall) :
		Light(l2w) {
	mPos = lightToWorld(Point(0, 0, 0));
	mIntensity = intensity;
	mCosMaxWidth = cosf(Radians(width));
	mCosFall = cosf(Radians(fall));
}

bool SpotLight::IsDeltaLight() const {
	return true;
}

RGB SpotLight::Sample_L(const Point &p, float pEpsilon, const LightSample &ls,
		Vector *wi, float *pdf, VisibilityTester *vis) const {
	*wi = Normalize(mPos - p); //标准化的点到光源的向量
	*pdf = 1.0f;
	vis->SetSegment(p, pEpsilon, mPos, 0.0f);
	return mIntensity * Falloff(-*wi) / DistanceSqr(mPos, p);
}

//2PI(1-cos(theta))= w
RGB SpotLight::Power(const Scene* scene) const {
	return mIntensity * 2.0f * M_PI * (1.0f - 0.5f * (mCosFall + mCosMaxWidth));
}

RGB SpotLight::Sample_L(const Scene *scene, const LightSample &ls, float u1,
		float u2, Ray *ray, Normal *Ns, float *pdf) const {
	assert(false);
	return 0;
}

