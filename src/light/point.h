/*
 * point.h
 *
 *  Created on: 2016年7月11日
 *      Author: Administrator
 */

#ifndef LIGHT_POINT_H_
#define LIGHT_POINT_H_

#include <rgb.h>
#include "kumo.h"
#include "light.h"
class PointLight:public Light{
public:
	PointLight(const Transform& l2w,const RGB& intensity):Light(l2w){
		mPos=lightToWorld(Point(0,0,0));
		mIntensity=intensity;
	};

	virtual RGB Sample_L(const Point &p, Float pEpsilon, const LightSample &ls, Vector3f *wi, Float *pdf,
				VisibilityTester *vis) const override;
	virtual Float Pdf(const Point &p, const Vector3f &wi) const override{
		return 0;
	}
	bool IsDeltaLight() const override;
	RGB Power(const Scene* scene) const override;

	virtual RGB Sample_L(const Scene *scene, const LightSample &ls,
		                              Float u1, Float u2,Ray *ray,
		                              Normal *Ns, Float *pdf) const override;
private:
	RGB mIntensity;//光源强度
	Point mPos;
};


#endif /* LIGHT_POINT_H_ */
