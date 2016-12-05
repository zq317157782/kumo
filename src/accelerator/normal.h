/*
 * normal.h
 *
 *  Created on: 2016年8月26日
 *      Author: zhuqian
 */

#ifndef SRC_ACCELERATOR_NORMAL_H_
#define SRC_ACCELERATOR_NORMAL_H_

#include "../core/seidennki.h"
#include "geometry.h"
#include "primitive.h"

class NormalAggregate: public Aggregate {
private:
	vector<Reference<Primitive>> mPrimitives;
	BBox mBounds;
public:
	NormalAggregate(const vector<Reference<Primitive>>& prims) {
		for (unsigned int i = 0; i < prims.size(); ++i) {
			prims[i]->FullyRefine(mPrimitives);
		}
		for (unsigned int i = 0; i < mPrimitives.size(); ++i) {
			BBox b = Union(mBounds, mPrimitives[i]->WorldBound()); //做并联
			mBounds = b;
		}
	}

	bool Intersect(const Ray &r, Intersection *in) const {
		bool flag = false;
		for (int i = 0; i < mPrimitives.size(); ++i) {
			if (mPrimitives[i]->Intersect(r, in)) {
				flag = true;
			}
		}
		return flag;
	}

	bool IntersectP(const Ray &ray) const override {
		for (int i = 0; i < mPrimitives.size(); ++i) {
			if (mPrimitives[i]->IntersectP(ray)) {
				return true;
			}
		}
		return false;
	}

	BBox WorldBound() const override {
		return mBounds;
	}
};

#endif /* SRC_ACCELERATOR_NORMAL_H_ */
