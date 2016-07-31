/*
 * diffuse.h
 *
 *  Created on: 2016年7月26日
 *      Author: Administrator
 */

#ifndef LIGHT_DIFFUSE_H_
#define LIGHT_DIFFUSE_H_
#include "global.h"
#include "light.h"

//漫反射区域光

class DiffuseAreaLight: public AreaLight {
private:
	RGB mLemit; //自发光
	Reference<Shape> mShape; //光源形状
	float mArea;
public:
	DiffuseAreaLight(const Transform& l2w, const RGB& le, int ns,
			const Reference<Shape> shape) :
			AreaLight(l2w, ns) {
		mLemit = le;
		mShape = shape;
		mArea = mShape->Area();
	}

	virtual RGB L(const Point &p, const Normal &n, const Vector &w) const
			override {
		return Dot(n, w) > 0.f ? mLemit : 0.f;
	}
	virtual RGB Power(const Scene* scene) const override {
		return mLemit * mArea * M_PI;
	}
	//TODO 还未实现
	virtual RGB Sample_L(const Point &p, float pEpsilon, const LightSample &ls,
			Vector *wi, float *pdf, VisibilityTester *vis) const override {

	}

	virtual bool IsDeltaLight() const override {
		return false;
	}

	virtual float Pdf(const Point &p, const Vector &wi) const override{
		assert(false);
	}
};

#endif /* LIGHT_DIFFUSE_H_ */
