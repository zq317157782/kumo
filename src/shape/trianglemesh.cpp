/*
 * trianglemesh.cpp
 *
 *  Created on: 2016年7月8日
 *      Author: Administrator
 */
#include "trianglemesh.h"
#include "transform.h"

TriangleMesh::TriangleMesh(const Transform *o2w, const Transform *w2o, bool ro,
		int ntris, int nverts, const int *vi, const Point *P, const Normal *N,
		const Vector *S, const float *uv) :
		Shape(o2w, w2o, ro) {
	this->ntris = ntris;
	this->nverts = nverts;
	vertexIndex = new int[3 * ntris]; //为索引数组分配内存空间
	memcpy(vertexIndex, vi, 3 * ntris * sizeof(int));

	//为顶点分配空间
	p = new Point[nverts];
	for (int i = 0; i < nverts; ++i) {
		p[i] = (*localToWorld)(P[i]);
//		cout << "Point=>x " << p[i].x << " y " << p[i].y << " z " << p[i].z
//				<< endl;
	}

	//为法线分配空间
	if (N) {
		n = new Normal[nverts];
		memcpy(n, N, nverts * sizeof(Normal));
	} else
		n = nullptr;

	//为切向量分配空间
	if (S) {
		s = new Vector[nverts];
		memcpy(s, S, nverts * sizeof(Vector));
	} else
		s = nullptr;

	//为uv坐标分配空间
	if (uv) {
		uvs = new float[2 * nverts];
		memcpy(uvs, uv, 2 * nverts * sizeof(float));
	} else
		uvs = nullptr;

}

void TriangleMesh::Refine(vector<Reference<Shape> > &refined) const {
	for (int i = 0; i < ntris; ++i)
		refined.push_back(
				new Triangle(localToWorld, worldToLocal, ReverseOrientation,
						(TriangleMesh *) this, i));
}

Triangle::Triangle(const Transform *o2w, const Transform *w2o, bool ro,
		TriangleMesh *m, int n) :
		Shape(o2w, w2o, ro) {
	mMesh = m;
	mIndex = &(mMesh->vertexIndex[3 * n]); //注意 这里是取地址
}

void Triangle::GetUVs(float uv[3][2]) const {
	if (mMesh->uvs) {
		uv[0][0] = mMesh->uvs[2 * mIndex[0]];
		uv[0][1] = mMesh->uvs[2 * mIndex[0] + 1];
		uv[1][0] = mMesh->uvs[2 * mIndex[1]];
		uv[1][1] = mMesh->uvs[2 * mIndex[1] + 1];
		uv[2][0] = mMesh->uvs[2 * mIndex[2]];
		uv[2][1] = mMesh->uvs[2 * mIndex[2] + 1];
	} else {
		uv[0][0] = 0.0f;
		uv[0][1] = 0.0f;
		uv[1][0] = 1.0f;
		uv[1][1] = 0.0f;
		uv[2][0] = 1.0f;
		uv[2][1] = 1.0f;
	}
}

bool Triangle::Intersect(const Ray &ray, float *distance, float *rayEpsilon,
		DifferentialGeometry *dg) const {
	Point p1 = mMesh->p[mIndex[0]];
	Point p2 = mMesh->p[mIndex[1]];
	Point p3 = mMesh->p[mIndex[2]];
	//这里使用质心坐标来计算 详见PBRT公式
	Vector e1 = p2 - p1; //e1=p2-p1
	Vector e2 = p3 - p1; //e2=p3-p1
	Vector s1 = Cross(ray.d, e2); //s1=d x e2
	float divisor = Dot(s1, e1);
	if (divisor == 0.0f)
		return false;
	float invDivisor = 1.f / divisor; //1/(s1.e1)

	// 计算第一个质心坐标
	Vector s = ray.o - p1; //s = o - p1
	float b1 = Dot(s, s1) * invDivisor; //b1 = (s.s1)/(s1.e1)
	if (b1 < 0. || b1 > 1.)
		return false;

	// 计算第二个质心坐标
	Vector s2 = Cross(s, e1); //s2 = s x e1
	float b2 = Dot(ray.d, s2) * invDivisor; // b2 = (d.s2)/(s1.e1)
	if (b2 < 0. || b1 + b2 > 1.)
		return false;

	// 计算参数t
	float t = Dot(e2, s2) * invDivisor;
	if (t < ray.minT || t > ray.maxT)
		return false;

	Vector dpdu, dpdv; //p在u和v上的偏导
	float uvs[3][2];
	GetUVs(uvs);

	//计算p在u和v上的偏导
	float du1 = uvs[0][0] - uvs[2][0];
	float du2 = uvs[1][0] - uvs[2][0];
	float dv1 = uvs[0][1] - uvs[2][1];
	float dv2 = uvs[1][1] - uvs[2][1];
	Vector dp1 = p1 - p3, dp2 = p2 - p3;
	float determinant = du1 * dv2 - dv1 * du2;
	if (determinant == 0.f) {
		//行列式为0
		CoordinateSystem(Normalize(Cross(e2, e1)), &dpdu, &dpdv);
	} else {
		float invdet = 1.f / determinant;
		dpdu = (dv2 * dp1 - dv1 * dp2) * invdet;
		dpdv = (-du2 * dp1 + du1 * dp2) * invdet;
	}
	//这里是在计算参数坐标  用的也是质心坐标
	float b0 = 1 - b1 - b2;
	float tu = b0 * uvs[0][0] + b1 * uvs[1][0] + b2 * uvs[2][0];
	float tv = b0 * uvs[0][1] + b1 * uvs[1][1] + b2 * uvs[2][1];

	*dg = DifferentialGeometry(ray(t), dpdu, dpdv, Normal(0, 0, 0),
			Normal(0, 0, 0), tu, tv, this);
	*distance = t;
	*rayEpsilon = 1e-3f * *distance;
	//cout<<"射中三角"<<endl;
	return true;
}

bool Triangle::IntersectP(const Ray& ray) const {
	Point p1 = mMesh->p[mIndex[0]];
	Point p2 = mMesh->p[mIndex[1]];
	Point p3 = mMesh->p[mIndex[2]];
	//这里使用质心坐标来计算 详见PBRT公式
	Vector e1 = p2 - p1; //e1=p2-p1
	Vector e2 = p3 - p1; //e2=p3-p1
	Vector s1 = Cross(ray.d, e2); //s1=d x e2
	float divisor = Dot(s1, e1);
	if (divisor == 0.0f)
		return false;
	float invDivisor = 1.f / divisor; //1/(s1.e1)

	// 计算第一个质心坐标
	Vector s = ray.o - p1; //s = o - p1
	float b1 = Dot(s, s1) * invDivisor; //b1 = (s.s1)/(s1.e1)
	if (b1 < 0. || b1 > 1.)
		return false;

	// 计算第二个质心坐标
	Vector s2 = Cross(s, e1); //s2 = s x e1
	float b2 = Dot(ray.d, s2) * invDivisor; // b2 = (d.s2)/(s1.e1)
	if (b2 < 0. || b1 + b2 > 1.)
		return false;

	// 计算参数t
	float t = Dot(e2, s2) * invDivisor;
	if (t < ray.minT || t > ray.maxT)
		return false;
	return true;
}

//通过两个向量的×乘的长度的一般来计算 面积
float Triangle::Area() const {
	const Point &p1 = mMesh->p[mIndex[0]];
	const Point &p2 = mMesh->p[mIndex[1]];
	const Point &p3 = mMesh->p[mIndex[2]];
	return  Cross(p2 - p1, p3 - p1).Length()*0.5f;
}

BBox Triangle::ObjectBound() const {
    const Point &p1 = mMesh->p[mIndex[0]];
    const Point &p2 = mMesh->p[mIndex[1]];
    const Point &p3 = mMesh->p[mIndex[2]];
    return Union(BBox((*worldToLocal)(p1), (*worldToLocal)(p2)),
                 (*worldToLocal)(p3));
}


BBox Triangle::WorldBound() const {
    const Point &p1 = mMesh->p[mIndex[0]];
    const Point &p2 = mMesh->p[mIndex[1]];
    const Point &p3 = mMesh->p[mIndex[2]];
    return Union(BBox(p1, p2), p3);
}
