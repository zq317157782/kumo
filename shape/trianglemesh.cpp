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
	for (int i = 0; i < nverts; ++i)
	        p[i] = (*localToWorld)(P[i]);

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
		s = NULL;

	//为uv坐标分配空间
	if (uv) {
		uvs = new float[2 * nverts];
		memcpy(uvs, uv, 2 * nverts * sizeof(float));
	} else
		uvs = nullptr;

}


Triangle::Triangle(const Transform *o2w, const Transform *w2o, bool ro, TriangleMesh *m, int n):Shape(o2w,w2o,ro){
	mMesh=m;
	mIndex=&(mMesh->vertexIndex[3*n]);//注意 这里是取地址
}
