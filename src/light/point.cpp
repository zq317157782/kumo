/*
 * light.cpp
 *
 *  Created on: 2016年7月11日
 *      Author: Administrator
 */
#include "point.h"
RGB PointLight::Sample_L(const Point &p, float pEpsilon, const LightSample &ls, Vector *wi, float *pdf,
				VisibilityTester *vis) const{
		*wi= Normalize(mPos-p);//标准化的点到光源的向量
		*pdf=1.0f;
		vis->SetSegment(p, pEpsilon, mPos, 0.0f);
		return mIntensity/DistanceSqr(mPos,p);
}



bool PointLight::IsDeltaLight() const{
	return true;
}


RGB PointLight::Power(const Scene* scene) const{
	return 4*M_PI*mIntensity; //对点光源在整个球体立体角内求积分
}
