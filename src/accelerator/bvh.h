/*
 * bvh.h
 *
 *  Created on: 2016年9月2日
 *      Author: zhuqian
 */

#ifndef SRC_ACCELERATOR_BVH_H_
#define SRC_ACCELERATOR_BVH_H_

#include "../core/seidennki.h"
#include "primitive.h"

struct BVHPrimitiveInfo {
	int index;
	Point centroid;
	BBox bounds;
	BVHPrimitiveInfo(int id, const BBox& b) :
			index(id), bounds(b) {
		centroid = b.pMin * 0.5f + b.pMax * 0.5f;
	}
};

//用于构建树的节点
struct BVHBuildNode {
	BBox bounds;
	BVHBuildNode* children[2]; //2个子节点
	unsigned int splitAxis; //分割的轴
	unsigned int offsetPrimitive; //在原始图元数组中的偏移
	unsigned int numPrimitives; //图元个数

	BVHBuildNode() {
		splitAxis = offsetPrimitive = numPrimitives = 0;
		children[0] = children[1] = nullptr;
	}

	//初始化为叶子节点
	void InitLeaf(unsigned int first, unsigned int n, const BBox& b) {
		offsetPrimitive = first;
		numPrimitives = n;
		bounds = b;
	}

	//初始化为中间节点
	void InitInterior(unsigned int axis, BVHBuildNode* c1, BVHBuildNode* c2) {
		children[0] = c1;
		children[1] = c2;
		bounds = Union(c1->bounds, c2->bounds);
		splitAxis = axis;
		numPrimitives = 0;
	}
};

struct CompareToMid {
	CompareToMid(int d, float m) {
		dim = d;
		mid = m;
	}
	int dim;
	float mid;
	bool operator()(const BVHPrimitiveInfo &a) const {
		return a.centroid[dim] < mid;
	}
};

struct ComparePoints {
	ComparePoints(int d) {
		dim = d;
	}
	int dim;
	bool operator()(const BVHPrimitiveInfo &a,
			const BVHPrimitiveInfo &b) const {
		return a.centroid[dim] < b.centroid[dim];
	}
};

//紧凑线性版本的BVH Node
struct LinearBVHNode {
	BBox bounds; //32*6

//32
	union {
		unsigned int primitiveOffset;	//Leaf
		unsigned int secondOffset; 		//interior
	};
	unsigned char numPrimitives; 		//8
	unsigned char axis; //8
	unsigned char pad[2]; //16
};

//BVH加速结构
class BVHAccel: public Aggregate {
public:
	enum SplitMethod {
		SPLIT_MIDDLE, SPLIT_EQUAL_COUNTS
	};
private:
	unsigned int mMaxPrimInNode; //每个node中最大的图元数
	vector<Reference<Primitive>> mPrimitives;
	SplitMethod mSplitMethod; //分割模式 目前支持两种,居中和等个数

	LinearBVHNode *mNodes;

	BVHBuildNode* recursiveBuild(MemoryArena& buildArena,
			vector<BVHPrimitiveInfo> &buildData, unsigned int start,
			unsigned int end, unsigned int * totalNodes,
			vector<Reference<Primitive> > &orderedPrims);
	unsigned int flattenBVHTree(BVHBuildNode *node, unsigned int *offset);
public:
	BVHAccel(const vector<Reference<Primitive>>& p, unsigned int maxPN,
			SplitMethod sm);

	BBox WorldBound() const override;
	bool Intersect(const Ray &r, Intersection *in) const override;
	bool IntersectP(const Ray &ray) const override;
};

#endif /* SRC_ACCELERATOR_BVH_H_ */
