/*
 * spot.h
 *
 *  Created on: 2016年7月12日
 *      Author: Administrator
 */

#ifndef LIGHT_SPOT_H_
#define LIGHT_SPOT_H_

#include <rgb.h>
#include "kumo.h"
#include "light.h"

class SpotLight:public Light{
private:
	RGB mIntensity;//光源强度
	Point mPos;
	Float mCosMaxWidth;
	Float mCosFall;
public:
	SpotLight(const Transform& l2w,const RGB& intensity,Float width,Float fall);
	bool IsDeltaLight() const override;
	virtual RGB Sample_L(const Point &p, Float pEpsilon, const LightSample &ls, Vector3f *wi, Float *pdf,
					VisibilityTester *vis) const override;
	virtual Float Pdf(const Point &p, const Vector3f &wi) const override{
		return 0;
	}
	//根据向量计算衰减
	Float Falloff(const Vector3f& w) const{
		Vector3f wl=Normalize(worldToLight(w)); //把世界坐标系下的向量转化成光源坐标系下
		Float costheta=wl.z;
		if(costheta<mCosMaxWidth) return 0;
		if(costheta>mCosFall) return 1;
		Float delta=(costheta-mCosMaxWidth)/(mCosFall-mCosMaxWidth);
		return delta*delta*delta*delta;//为什么这里要4次平方。
	}

	RGB Power(const Scene* scene) const override;

	virtual RGB Sample_L(const Scene *scene, const LightSample &ls,
			                              Float u1, Float u2,Ray *ray,
			                              Normal *Ns, Float *pdf) const override;
};


#endif /* LIGHT_SPOT_H_ */
