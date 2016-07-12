/*
 * spot.h
 *
 *  Created on: 2016年7月12日
 *      Author: Administrator
 */

#ifndef LIGHT_SPOT_H_
#define LIGHT_SPOT_H_

#include "global.h"
#include "light.h"
#include "RGB.h"

class SpotLight:public Light{
private:
	RGB mIntensity;//光源强度
	Point mPos;
	float mCosMaxWidth;
	float mCosFall;
public:
	SpotLight(const Transform& l2w,const RGB& intensity,float width,float fall);
	bool IsDeltaLight() const override;
	virtual RGB Sample_L(const Point &p, float pEpsilon, const LightSample &ls, Vector *wi, float *pdf,
					VisibilityTester *vis) const override;
	//根据向量计算衰减
	float Falloff(const Vector& w) const{
		Vector wl=Normalize(worldToLight(w)); //把世界坐标系下的向量转化成光源坐标系下
		float costheta=wl.z;
		if(costheta<mCosMaxWidth) return 0;
		if(costheta>mCosFall) return 1;
		float delta=(costheta-mCosMaxWidth)/(mCosFall-mCosMaxWidth);
		return delta*delta*delta*delta;//为什么这里要4次平方。
	}

	RGB Power(Scene* scene) const override;
};


#endif /* LIGHT_SPOT_H_ */
