/*
 * distant.h
 *
 *  Created on: 2016年7月12日
 *      Author: Administrator
 */

#ifndef LIGHT_DISTANT_H_
#define LIGHT_DISTANT_H_
#include "global.h"
#include "light.h"
#include "RGB.h"

class DistantLight:public Light{
private:
	RGB mRadiance;
	Vector mDir;
public:
	DistantLight(const Transform& l2w,const RGB& radiance,const Vector& dir):Light(l2w){
		mRadiance=radiance;
		mDir=Normalize(lightToWorld(dir));
	}

	//todo 完成方向光的Power函数
	RGB Power(const Scene* scene) const override{
		assert(false);
	}

	bool IsDeltaLight() const override{
		return true;
	}

	virtual RGB Sample_L(const Point &p, float pEpsilon, const LightSample &ls, Vector *wi, float *pdf,
						VisibilityTester *vis) const override{
		*wi=mDir;
		*pdf=1.0f;
		vis->SetRay(p,pEpsilon,*wi);
		return mRadiance;
	}

	virtual float Pdf(const Point &p, const Vector &wi) const override{
		return 0;
	}
};




#endif /* LIGHT_DISTANT_H_ */
