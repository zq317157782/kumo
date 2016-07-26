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
#include "memory.h"

//光源类
class Light: public ReferenceCounted {
protected:
	const Transform lightToWorld;
	const Transform worldToLight;
public:
	const int numSamples;
	Light(const Transform& l2w, int nsample = 1);
	virtual RGB Sample_L(const Point &p, float pEpsilon, const LightSample &ls,
			Vector *wi, float *pdf, VisibilityTester *vis) const = 0; //返回辐射照度
	virtual bool IsDeltaLight() const = 0;
	virtual RGB Power(Scene* scene) const=0; //返回光源产生的辐射通量
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

struct LightSample {

};

//区域光
class AreaLight: public Light {
public:
	AreaLight(const Transform& l2w, int nsample = 1):Light(l2w,nsample){

	}
	/***
	 *p 光源表面的一个点   n光源表面上的法线  w光线射向的方向
	 */
	virtual RGB L(const Point &p, const Normal &n, const Vector &w) const = 0;

	virtual bool IsDeltaLight() const override{
		return false;
	}
};
#endif /* CORE_LIGHT_H_ */
