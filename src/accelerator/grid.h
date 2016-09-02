/*
 * grid.h
 *
 *  Created on: 2016年8月19日
 *      Author: zhuqian
 */

#ifndef SRC_ACCELERATOR_GRID_H_
#define SRC_ACCELERATOR_GRID_H_
#include "global.h"
#include "primitive.h"
#include "parallel.h"

//空间体素
struct Voxel {

private:
	vector<Reference<Primitive>> mPrimitives;
	bool mAllCanIntersect;
public:
	Voxel() {
	}
	Voxel(Reference<Primitive> p) {
		mPrimitives.push_back(p);
		mAllCanIntersect = false;
	}

	void AddPrimitive(Reference<Primitive> p) {
		mPrimitives.push_back(p);
	}
	unsigned int size() {
		return mPrimitives.size();
	}

	bool Intersect(const Ray &ray, Intersection *isect,RWMutexLock& lock);
	bool IntersectP(const Ray &ray,RWMutexLock& lock);
};

//Grid 空间分割加速器
class GridAccel: public Aggregate {
private:
	mutable RWMutex rwlock;
	vector<Reference<Primitive>> mPrimitives;
	BBox mBounds;
	int mNumVoxels[3];
	Vector mWidth, mInvWidth; //每个体素的宽度
	Voxel ** mVoxels;
	MemoryArena mVoxelArena;//分配体素用内存
	int posToVoxel(const Point &P, int axis) const {
		int v = Float2Int((P[axis] - mBounds.pMin[axis]) * mInvWidth[axis]);
		return Clamp(v, 0, mNumVoxels[axis] - 1);
	}

	float voxelToPos(int p, int axis) const {
		return mBounds.pMin[axis] + p * mWidth[axis];
	}

	inline int offset(int x, int y, int z) const {
		return z * mNumVoxels[0] * mNumVoxels[1] + y * mNumVoxels[0] + x;
	}

	bool CanIntersect() const { return true; }

public:
	GridAccel(const vector<Reference<Primitive>>& prims,
			bool refineImmediately);
	bool Intersect(const Ray &r, Intersection *in) const override;
	BBox WorldBound() const override;
	bool IntersectP(const Ray &ray) const override;
};

#endif /* SRC_ACCELERATOR_GRID_H_ */
