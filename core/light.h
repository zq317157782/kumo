/*
 * light.h
 *
 *  Created on: 2016年7月11日
 *      Author: Administrator
 */

#ifndef CORE_LIGHT_H_
#define CORE_LIGHT_H_

#include "global.h"
#include "transform.h"

//光源类
class Light {
protected:
	const Transform lightToWorld;
	const Transform worldToLight;
public:
	const int numSamples;
	Light(const Transform& l2w, int nsample = 1);
	virtual RGB Sample_L(const Point &p, float pEpsilon, const LightSample &ls,
			float time, Vector *wi, float *pdf,
			VisibilityTester *vis) const = 0;
};

//用来判断两个点之间是否有遮挡，或者射线是否与中间图元相交
struct VisibilityTester {
	void SetSegment(const Point &p1, float eps1, const Point &p2, float eps2) {
		float dist = Distance(p1, p2);
		r = Ray(p1, (p2 - p1) / dist, eps1, dist * (1.f - eps2));
	}
	void SetRay(const Point &p, float eps, const Vector &w) {
		r = Ray(p, w, eps, INFINITY);
	}
	bool Unoccluded(const Scene *scene) const;
	Ray r;
};

#endif /* CORE_LIGHT_H_ */
