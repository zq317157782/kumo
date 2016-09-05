/*
 * bvh.cpp
 *
 *  Created on: 2016年9月2日
 *      Author: zhuqian
 */
#include "bvh.h"

//一种快速的Ray 和 Bound相交测试的方法 还没有完全理解
static inline bool IntersectP(const BBox &bounds, const Ray &ray,
		const Vector &invDir, const unsigned int dirIsNeg[3]) {

	float tmin = (bounds[dirIsNeg[0]].x - ray.o.x) * invDir.x;
	float tmax = (bounds[1 - dirIsNeg[0]].x - ray.o.x) * invDir.x;
	float tymin = (bounds[dirIsNeg[1]].y - ray.o.y) * invDir.y;
	float tymax = (bounds[1 - dirIsNeg[1]].y - ray.o.y) * invDir.y;
	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (bounds[dirIsNeg[2]].z - ray.o.z) * invDir.z;
	float tzmax = (bounds[1 - dirIsNeg[2]].z - ray.o.z) * invDir.z;
	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
	return (tmin < ray.maxT) && (tmax > ray.minT);
}

BVHAccel::BVHAccel(const vector<Reference<Primitive>>& p, unsigned int maxPN,
		SplitMethod sm) {
	this->mMaxPrimInNode = min(255u, maxPN); //最大个数为255 PBRT默认，我觉得到时候可以更改
	this->mSplitMethod = sm;
	for (unsigned int i = 0; i < p.size(); ++i) {
		p[i]->FullyRefine(mPrimitives); //完全Refine
	}

	if (mPrimitives.size() == 0) {
		mNodes = nullptr;
		return;
	}
	vector<BVHPrimitiveInfo> buildData; //用于构成TREE的中间结构
	buildData.reserve(mPrimitives.size());
	for (unsigned int i = 0; i < mPrimitives.size(); ++i) {
		BBox bbox = mPrimitives[i]->WorldBound();
		buildData.push_back(BVHPrimitiveInfo(i, bbox));
	}

	MemoryArena buildArena; //用于创建buildTree的内存分配
	unsigned int totalNodes = 0;
	vector<Reference<Primitive> > orderedPrims; //排序后的图元
	orderedPrims.reserve(mPrimitives.size());
	//生成buildTree 返回根节点
	BVHBuildNode *root = recursiveBuild(buildArena, buildData, 0,
			mPrimitives.size(), &totalNodes, orderedPrims);

	//交换排序过的数组
	mPrimitives.swap(orderedPrims);

	printf("BVH created with %d nodes for %d primitives (%.2f MB)", totalNodes,
			(int) mPrimitives.size(),
			float(totalNodes * sizeof(LinearBVHNode)) / (1024.f * 1024.f));

	mNodes = AllocAligned<LinearBVHNode>(totalNodes);
	//创建LinearBVHNode对象
	for (unsigned int i = 0; i < totalNodes; ++i) {
		new (&mNodes[i]) LinearBVHNode;
	}
	unsigned int offset = 0;
	//开始转换buildBVHTree到linearBVHTree;
	flattenBVHTree(root, &offset);
	assert(offset == totalNodes);
}

unsigned int BVHAccel::flattenBVHTree(BVHBuildNode *node,
		unsigned int *offset) {
	LinearBVHNode* linearNode = &mNodes[*offset]; //获得LinearNode
	linearNode->bounds = node->bounds; //赋值BBox
	unsigned int myOffset = (*offset)++; //自增offset

	if (node->numPrimitives > 0) {
		assert(!node->children[0] && !node->children[1]);
		//叶子节点
		linearNode->primitiveOffset = node->offsetPrimitive;
		linearNode->numPrimitives = node->numPrimitives;
	} else {
		//中间节点
		linearNode->axis = node->splitAxis;
		linearNode->numPrimitives = 0;
		//开始递归
		flattenBVHTree(node->children[0], offset);
		linearNode->secondOffset = flattenBVHTree(node->children[1], offset);
	}
	return myOffset;
}

BVHBuildNode* BVHAccel::recursiveBuild(MemoryArena& buildArena,
		vector<BVHPrimitiveInfo> &buildData, unsigned int start,
		unsigned int end, unsigned int * totalNodes,
		vector<Reference<Primitive> > &orderedPrims) {
	(*totalNodes)++;
	BVHBuildNode *node = buildArena.Alloc<BVHBuildNode>(); //为Node分配空间
	BBox bbox;
	for (int i = start; i < end; ++i) {
		bbox = Union(bbox, buildData[i].bounds);
	}

	unsigned int nPrimitives = end - start; //图元个数
	if (nPrimitives == 1) { //创建叶子
		unsigned int first = orderedPrims.size();
		for (unsigned int i = start; i < end; ++i) {
			unsigned int index = buildData[i].index;
			orderedPrims.push_back(mPrimitives[index]); //往orderedPrims中插入图元
		}
		node->InitLeaf(first, nPrimitives, bbox);
	} else { //创建中间点
		BBox centroidBounds;
		for (unsigned int i = start; i < end; ++i) {
			centroidBounds = Union(centroidBounds, buildData[i].centroid);
		}
		int dim = centroidBounds.MaximumExtent(); //获得最长的那个轴

		unsigned int mid = (start + end) / 2; //中间点索引
		if (centroidBounds.pMax[dim] == centroidBounds.pMin[dim]) { //unusual
			if (nPrimitives < mMaxPrimInNode) {
				//直接返回叶子
				unsigned int first = orderedPrims.size();
				for (unsigned int i = start; i < end; ++i) {
					unsigned int index = buildData[i].index;
					orderedPrims.push_back(mPrimitives[index]); //往orderedPrims中插入图元
				}
				node->InitLeaf(first, nPrimitives, bbox);
				return node;
			} else {
				//需要继续分割，直到每个node包含符合规定的图元数
				node->InitInterior(dim,
						recursiveBuild(buildArena, buildData, start, mid,
								totalNodes, orderedPrims),
						recursiveBuild(buildArena, buildData, mid, end,
								totalNodes, orderedPrims));
				return node;
			}

		} else { //usual
				 //根据分割模式 选择不同的分割方式
			switch (mSplitMethod) {
			case SPLIT_MIDDLE: {
				float pMid = (centroidBounds.pMin[dim]
						+ centroidBounds.pMax[dim]) * 0.5f;
				//根据比较函数排序数组，并且获得指向第一个不满足比较的元素的指针
				BVHPrimitiveInfo *midPtr = partition(&buildData[start],
						&buildData[end - 1] + 1, CompareToMid(dim, pMid));
				mid = midPtr - &buildData[0];				 //获得中点
				if (mid != start && mid != end)
					break;
			}
			case SPLIT_EQUAL_COUNTS: {
				mid = (start + end) / 2;				 //获得中点
				std::nth_element(&buildData[start], &buildData[mid],
						&buildData[end - 1] + 1, ComparePoints(dim));
				break;
			}
			}
			//初始化中间点 递归
			node->InitInterior(dim,
					recursiveBuild(buildArena, buildData, start, mid,
							totalNodes, orderedPrims),
					recursiveBuild(buildArena, buildData, mid, end, totalNodes,
							orderedPrims));

		}
	}
	return node;
}

BBox BVHAccel::WorldBound() const {
	return mNodes ? mNodes[0].bounds : BBox();
}
bool BVHAccel::Intersect(const Ray &ray, Intersection *isect) const {
	if (!mNodes)
		return false;
	bool hit = false;
	Vector invDir(1.0f / ray.d.x, 1.0f / ray.d.y, 1.0f / ray.d.z);		//方向翻转
	unsigned int dirIsNeg[3] = { invDir.x < 0, invDir.y < 0, invDir.z < 0 };//判断方向的原始是否为负数
	unsigned int todoOffset = 0, nodeNum = 0;
	unsigned int todo[64];
//开始循环遍历node
	while (true) {
		const LinearBVHNode *node = &mNodes[nodeNum];		//获取当前节点
		if (::IntersectP(node->bounds, ray, invDir, dirIsNeg)) {
			//射中node
			if (node->numPrimitives > 0) {
				//节点中有图元
				for (unsigned int i = 0; i < node->numPrimitives; ++i) {
					if (mPrimitives[node->primitiveOffset + i]->Intersect(ray,
							isect)) {
						hit = true;
					}
				}

				if (todoOffset == 0)
					break;
				nodeNum = todo[--todoOffset];
			} else {
				if (dirIsNeg[node->axis]) {
					//先遍历第二个子节点
					todo[todoOffset++] = nodeNum + 1;
					nodeNum = node->secondOffset;
				} else {
					todo[todoOffset++] = node->secondOffset;
					nodeNum = nodeNum + 1;
				}
			}
		} else {
			//没射中node
			if (todoOffset == 0)
				break;
			nodeNum = todo[--todoOffset];
		}
	}
	return hit;
}
bool BVHAccel::IntersectP(const Ray &ray) const {
	if (!mNodes)
		return false;
	Vector invDir(1.0f / ray.d.x, 1.0f / ray.d.y, 1.0f / ray.d.z);		//方向翻转
	unsigned int dirIsNeg[3] = { invDir.x < 0, invDir.y < 0, invDir.z < 0 };//判断方向的原始是否为负数
	unsigned int todoOffset = 0, nodeNum = 0;
	unsigned int todo[64];
	//开始循环遍历node
	while (true) {
		const LinearBVHNode *node = &mNodes[nodeNum];		//获取当前节点
		if (::IntersectP(node->bounds, ray, invDir, dirIsNeg)) {
			//射中node
			if (node->numPrimitives > 0) {
				//节点中有图元
				for (unsigned int i = 0; i < node->numPrimitives; ++i) {
					if (mPrimitives[node->primitiveOffset + i]->IntersectP(
							ray)) {
						return true;
					}
				}

				if (todoOffset == 0)
					break;
				nodeNum = todo[--todoOffset];
			} else {
				if (dirIsNeg[node->axis]) {
					//先遍历第二个子节点
					todo[todoOffset++] = nodeNum + 1;
					nodeNum = node->secondOffset;
				} else {
					todo[todoOffset++] = node->secondOffset;
					nodeNum = nodeNum + 1;
				}
			}
		} else {
			//没射中node
			if (todoOffset == 0)
				break;
			nodeNum = todo[--todoOffset];
		}
	}
	return false;
}

