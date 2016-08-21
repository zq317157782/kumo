/*
 * grid.cpp
 *
 *  Created on: 2016年8月19日
 *      Author: zhuqian
 */

#include "grid.h"
#include "parallel.h"
#include "geometry.h"
bool Voxel::Intersect(const Ray &ray, Intersection *isect,RWLock& lock) {
	if (!mAllCanIntersect) { //判断所有的图元都是否可交
		lock.upgrade2Writer();
		for (unsigned int i = 0; i < mPrimitives.size(); ++i) {
			Reference<Primitive> &prim = mPrimitives[i];
			if (!prim->CanIntersect()) {
				vector<Reference<Primitive> > p;
				prim->FullyRefine(p);
				assert(p.size() > 0);
				if (p.size() == 1)
					mPrimitives[i] = p[0];
				else
					mPrimitives[i] = new GridAccel(p, false);//生成多个图元的话，直接重新构建一个新的Grid
			}
		}
		mAllCanIntersect = true;
		lock.down2Reader();
	}
	bool hitSomething = false;
	for (unsigned i = 0; i < mPrimitives.size(); ++i) {
		Reference<Primitive> &prim = mPrimitives[i];
		if (prim->Intersect(ray, isect)) {
			hitSomething = true;
		}
	}
	return hitSomething;
}

bool Voxel::IntersectP(const Ray &ray,RWLock& lock){
	if (!mAllCanIntersect) { //判断所有的图元都是否可交
			lock.upgrade2Writer();
			for (unsigned int i = 0; i < mPrimitives.size(); ++i) {
				Reference<Primitive> &prim = mPrimitives[i];
				if (!prim->CanIntersect()) {
					vector<Reference<Primitive> > p;
					prim->FullyRefine(p);
					assert(p.size() > 0);
					if (p.size() == 1)
						mPrimitives[i] = p[0];
					else
						mPrimitives[i] = new GridAccel(p, false);//生成多个图元的话，直接重新构建一个新的Grid
				}
			}
			mAllCanIntersect = true;
			lock.down2Reader();
		}
		bool hitSomething = false;
		for (unsigned i = 0; i < mPrimitives.size(); ++i) {
			Reference<Primitive> &prim = mPrimitives[i];
			if (prim->IntersectP(ray)) {
				hitSomething = true;
			}
		}
		return hitSomething;
}

GridAccel::GridAccel(const vector<Reference<Primitive>>& prims,
		bool refineImmediately) {
	mBounds=BBox(Point());
	if (refineImmediately) {
		for (unsigned int i = 0; i < prims.size(); ++i) {
			prims[i]->FullyRefine(mPrimitives);
		}
	} else {
		mPrimitives = prims;
	}

	for (unsigned int i = 0; i < mPrimitives.size(); ++i) {
		mBounds = Union(mBounds, mPrimitives[i]->WorldBound()); //做并联
	}
	Vector delta = mBounds.pMax - mBounds.pMin;

	int maxAxis = mBounds.MaximumExtent();
	float invMaxWidth = 1 / delta[maxAxis];
	float cubeRoot = powf(float(mPrimitives.size()), 1.0f / 3.0f) * 3.0f;
	float voxelPerUnit = cubeRoot * invMaxWidth;

	//计算获取每个轴体素的个数
	for (int axis = 0; axis < 3; ++axis) {
		mNumVoxels[axis] = Round2Int(delta[axis] * voxelPerUnit);
		mNumVoxels[axis] = Clamp(mNumVoxels[axis], 1, 64); //裁剪在1~64的范围内
	}

	for (int axis = 0; axis < 3; ++axis) {
		mWidth[axis] = delta[axis] / mNumVoxels[axis];
		mInvWidth[axis] = (mWidth[axis] == 0) ? 0.0f : 1.0f / mWidth[axis];
	}
	int numVoxels = mNumVoxels[0] * mNumVoxels[1] * mNumVoxels[2];
	mVoxels = AllocAligned<Voxel*>(numVoxels);
	memset(mVoxels, 0, numVoxels * sizeof(Voxel *)); //分配体素的内存空间
	//开始填充图元
	for (unsigned int i = 0; i < mPrimitives.size(); ++i) {
		BBox pb = mPrimitives[i]->WorldBound();
		int vMax[3], vMin[3];
		for (int axis = 0; axis < 3; ++axis) {
			vMin[axis] = posToVoxel(pb.pMin, axis);
			vMax[axis] = posToVoxel(pb.pMax, axis);
		}
		//向需要填充的VOXEL中填充图元
		for (int z = vMin[2]; z <= vMax[2]; ++z) {
			for (int y = vMin[1]; y <= vMax[1]; ++y) {
				for (int x = vMin[0]; x <=vMax[0]; ++x) {
					int o = offset(x, y, z);
					if (mVoxels[o]) {
						mVoxels[o]->AddPrimitive(mPrimitives[i]);

					} else {
						mVoxels[o] = mVoxelArena.Alloc<Voxel>();
						*mVoxels[o] = Voxel(mPrimitives[i]);
					}
				}
			}
		}
	}
}

bool GridAccel::Intersect(const Ray &r, Intersection *in) const {
	float rayT;
	//判断是否在内部
	if (mBounds.Inside(r(r.minT))) {
		rayT = r.minT;
	}
	//判断是否相交
	else if (!mBounds.IntersectP(r, &rayT)) {
		return false;
	}
	Point gridIntersect = r(rayT);	//和Grid的bound的相交点

	float NextCrossingT[3], DeltaT[3];
	int Step[3], Out[3], Pos[3];
	for (int axis = 0; axis < 3; ++axis) {
		Pos[axis] = posToVoxel(gridIntersect, axis);
		if (r.d[axis] >= 0) {
			NextCrossingT[axis] = rayT
					+ (voxelToPos(Pos[axis] + 1, axis) - gridIntersect[axis])
							/ r.d[axis];
			DeltaT[axis] = mWidth[axis] / r.d[axis];
			Step[axis] = 1;
			Out[axis] = mNumVoxels[axis];
		} else {
			NextCrossingT[axis] = rayT
					+ (voxelToPos(Pos[axis], axis) - gridIntersect[axis])
							/ r.d[axis];
			DeltaT[axis] = -mWidth[axis] / r.d[axis];
			Step[axis] = -1;
			Out[axis] = -1;
		}
	}
	RWLock lock;
	lock.readLock();
	bool hitSomething = false;
	for (;;) {
		Voxel *voxel = mVoxels[offset(Pos[0], Pos[1], Pos[2])];
		if (voxel != nullptr)
			hitSomething |= voxel->Intersect(r, in,lock);
		//以下采用了位操作来选取最近交点的轴，具体为什么这样子，我也不知道，只是为了效率这样做。
		int bits = ((NextCrossingT[0] < NextCrossingT[1]) << 2)
				+ ((NextCrossingT[0] < NextCrossingT[2]) << 1)
				+ ((NextCrossingT[1] < NextCrossingT[2]));
		const int cmpToAxis[8] = { 2, 1, 2, 1, 2, 2, 0, 0 };
		int stepAxis = cmpToAxis[bits];
		if (r.maxT < NextCrossingT[stepAxis])//最近的交点已经小于射线的最大参数了
			break;
		Pos[stepAxis] += Step[stepAxis];//在stepAxix轴上步进
		if (Pos[stepAxis] == Out[stepAxis])//已经走到了Grid外边
			break;
		NextCrossingT[stepAxis] += DeltaT[stepAxis];//更新下一个在stepAxis上相交的参数坐标
	}
	lock.readUnlock();
	//cout<<hitSomething<<endl;
	return hitSomething;
}

BBox GridAccel::WorldBound() const {
	return mBounds;
}

bool GridAccel::IntersectP(const Ray &r) const {
	float rayT;
		//判断是否在内部
		if (mBounds.Inside(r(r.minT))) {
			rayT = r.minT;
		}
		//判断是否相交
		else if (!mBounds.IntersectP(r, &rayT)) {
			return false;
		}
		Point gridIntersect = r(rayT);	//和Grid的bound的相交点

		float NextCrossingT[3], DeltaT[3];
		int Step[3], Out[3], Pos[3];
		for (int axis = 0; axis < 3; ++axis) {
			Pos[axis] = posToVoxel(gridIntersect, axis);
			if (r.d[axis] >= 0) {
				NextCrossingT[axis] = rayT
						+ (voxelToPos(Pos[axis] + 1, axis) - gridIntersect[axis])
								/ r.d[axis];
				DeltaT[axis] = mWidth[axis] / r.d[axis];
				Step[axis] = 1;
				Out[axis] = mNumVoxels[axis];
			} else {
				NextCrossingT[axis] = rayT
						+ (voxelToPos(Pos[axis], axis) - gridIntersect[axis])
								/ r.d[axis];
				DeltaT[axis] = -mWidth[axis] / r.d[axis];
				Step[axis] = -1;
				Out[axis] = -1;
			}
		}
		RWLock lock;
		lock.readLock();
		for (;;) {
			Voxel *voxel = mVoxels[offset(Pos[0], Pos[1], Pos[2])];
			if (voxel&&voxel->IntersectP(r,lock))
				return true;
			//以下采用了位操作来选取最近交点的轴，具体为什么这样子，我也不知道，只是为了效率这样做。
			int bits = ((NextCrossingT[0] < NextCrossingT[1]) << 2)
					+ ((NextCrossingT[0] < NextCrossingT[2]) << 1)
					+ ((NextCrossingT[1] < NextCrossingT[2]));
			const int cmpToAxis[8] = { 2, 1, 2, 1, 2, 2, 0, 0 };
			int stepAxis = cmpToAxis[bits];
			if (r.maxT < NextCrossingT[stepAxis])//最近的交点已经小于射线的最大参数了
				break;
			Pos[stepAxis] += Step[stepAxis];//在stepAxix轴上步进
			if (Pos[stepAxis] == Out[stepAxis])//已经走到了Grid外边
				break;
			NextCrossingT[stepAxis] += DeltaT[stepAxis];//更新下一个在stepAxis上相交的参数坐标
		}
		lock.readUnlock();
		return false;
}
