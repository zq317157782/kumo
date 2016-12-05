/*
 * trianglemesh.h
 *
 *  Created on: 2016年7月8日
 *      Author: Administrator
 */

#ifndef SHAPE_TRIANGLEMESH_H_
#define SHAPE_TRIANGLEMESH_H_

#include <shape.h>
#include "kumo.h"
//三角网格
class TriangleMesh: public Shape {
protected:
	int ntris, nverts;
	int *vertexIndex; //索引数组
	int *normalIndex; //索引数组
	int *texIndex; //索引数组
	Point *p; //顶点数组
	Normal *n; //法线数组
	Vector *s; //切向量数组
	Float *uvs; //纹理坐标数组
public:
	TriangleMesh(const Transform *o2w, const Transform *w2o, bool ro, int ntris,
			int nverts, const int *vi, const Point *P, const Normal *N,
			const Vector *S, const Float *uv,const int *ni=nullptr,const int *ti=nullptr);
	virtual ~TriangleMesh() {
	}
	;
	virtual void Refine(vector<Reference<Shape> > &refined) const override;
	virtual bool CanIntersect() const override {
		return false;
	}
	virtual BBox ObjectBound() const override {
		BBox objectBounds;
		for (int i = 0; i < nverts; i++)
			objectBounds = Union(objectBounds, (*worldToLocal)(p[i]));
		return objectBounds;
	}
	virtual BBox WorldBound() const override{
		BBox worldBounds;
		for (int i = 0; i < nverts; i++)
			worldBounds = Union(worldBounds, p[i]);
		return worldBounds;
	}
	friend class Triangle;
};

//三角形
class Triangle: public Shape {
private:
	Reference<TriangleMesh> mMesh;
	int *mIndex;
	int *mNormalIndex;
	int *mTexIndex;
public:
	Triangle(const Transform *o2w, const Transform *w2o, bool ro,
			TriangleMesh *m, int n);

	//获取三个顶点的纹理坐标
	void GetUVs(Float uv[3][2]) const;
	virtual bool Intersect(const Ray &ray, Float *distance, Float *rayEpsilon,
			DifferentialGeometry *dg) const override;
	virtual bool IntersectP(const Ray& ray) const override;
	virtual bool CanIntersect() const override {
		return true;
	}
	virtual Float Area() const override;
	virtual BBox ObjectBound() const override;
	BBox WorldBound() const;
};

#endif /* SHAPE_TRIANGLEMESH_H_ */
