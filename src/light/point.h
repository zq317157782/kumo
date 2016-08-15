/*
 * point.h
 *
 *  Created on: 2016年7月11日
 *      Author: Administrator
 */

#ifndef LIGHT_POINT_H_
#define LIGHT_POINT_H_

#include "global.h"
#include "light.h"
#include "RGB.h"
class PointLight:public Light{
public:
	PointLight(const Transform& l2w,const RGB& intensity):Light(l2w){
		mPos=lightToWorld(Point(0,0,0));
		mIntensity=intensity;
	};

	virtual RGB Sample_L(const Point &p, float pEpsilon, const LightSample &ls, Vector *wi, float *pdf,
				VisibilityTester *vis) const override;
	virtual float Pdf(const Point &p, const Vector &wi) const override{
		return 0;
	}
	bool IsDeltaLight() const override;
	RGB Power(const Scene* scene) const override;

	virtual RGB Sample_L(const Scene *scene, const LightSample &ls,
		                              float u1, float u2,Ray *ray,
		                              Normal *Ns, float *pdf) const override;
private:
	RGB mIntensity;//光源强度
	Point mPos;
};


#endif /* LIGHT_POINT_H_ */
