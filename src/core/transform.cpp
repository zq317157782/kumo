//
// Created by 诸谦 on 16/6/9.
//

#include "global.h"
#include "transform.h"

bool SolveLinearSystem2x2(const float A[2][2], const float B[2], float *x0,
		float *x1) {
	float det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
	if (fabsf(det) < 1e-10f)
		return false;
	*x0 = (A[1][1] * B[0] - A[0][1] * B[1]) / det;
	*x1 = (A[0][0] * B[1] - A[1][0] * B[0]) / det;
	if (isnan(*x0) || isnan(*x1))
		return false;
	return true;
}

Matrix4X4::Matrix4X4(float mm[4][4]) {
	memcpy(m, mm, 16 * sizeof(float));
}

Matrix4X4 Transpose(const Matrix4X4 &mm) {
	return Matrix4X4(mm.m[0][0], mm.m[1][0], mm.m[2][0], mm.m[3][0], mm.m[0][1],
			mm.m[1][1], mm.m[2][1], mm.m[3][1], mm.m[0][2], mm.m[1][2],
			mm.m[2][2], mm.m[3][2], mm.m[0][3], mm.m[1][3], mm.m[2][3],
			mm.m[3][3]);
}

//求逆矩阵 使用高斯-约旦法 代码取自PBRT
Matrix4X4 Inverse(const Matrix4X4 &mm) {
	int indxc[4], indxr[4];
	int ipiv[4] = { 0, 0, 0, 0 };
	float minv[4][4];
	memcpy(minv, mm.m, 4 * 4 * sizeof(float));
	for (int i = 0; i < 4; i++) {
		int irow = -1, icol = -1;
		float big = 0.;
		// Choose pivot
		for (int j = 0; j < 4; j++) {
			if (ipiv[j] != 1) {
				for (int k = 0; k < 4; k++) {
					if (ipiv[k] == 0) {
						if (fabsf(minv[j][k]) >= big) {
							big = float(fabsf(minv[j][k]));
							irow = j;
							icol = k;
						}
					} else if (ipiv[k] > 1)
						throw "Singular matrix in MatrixInvert";

					// Error("Singular matrix in MatrixInvert");
				}
			}
		}
		++ipiv[icol];
		// Swap rows _irow_ and _icol_ for pivot
		if (irow != icol) {
			for (int k = 0; k < 4; ++k)
				swap(minv[irow][k], minv[icol][k]);
		}
		indxr[i] = irow;
		indxc[i] = icol;
		if (minv[icol][icol] == 0.)
			throw "Singular matrix in MatrixInvert";
		//Error("Singular matrix in MatrixInvert");

		// Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
		float pivinv = 1.f / minv[icol][icol];
		minv[icol][icol] = 1.f;
		for (int j = 0; j < 4; j++)
			minv[icol][j] *= pivinv;

		// Subtract this row from others to zero out their columns
		for (int j = 0; j < 4; j++) {
			if (j != icol) {
				float save = minv[j][icol];
				minv[j][icol] = 0;
				for (int k = 0; k < 4; k++)
					minv[j][k] -= minv[icol][k] * save;
			}
		}
	}
	// Swap columns to reflect permutation
	for (int j = 3; j >= 0; j--) {
		if (indxr[j] != indxc[j]) {
			for (int k = 0; k < 4; k++)
				swap(minv[k][indxr[j]], minv[k][indxc[j]]);
		}
	}
	return Matrix4X4(minv);
}

Vector Transform::operator()(const Vector &v) const {
	float x = v.x, y = v.y, z = v.z;
	return Vector(m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z,
			m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z,
			m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z);
}

void Transform::operator()(const Vector &v, Vector *rv) const {
	float x = v.x, y = v.y, z = v.z;
	rv->x = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z;
	rv->y = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z;
	rv->z = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z;
}

Point Transform::operator()(const Point &p) const {
	float x = p.x, y = p.y, z = p.z;
	float xp = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3];
	float yp = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3];
	float zp = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3];
	float wp = m.m[3][0] * x + m.m[3][1] * y + m.m[3][2] * z + m.m[3][3];
	assert(wp != 0.0f);
	if (wp == 1.)
		return Point(xp, yp, zp);
	else
		return Point(xp, yp, zp) / wp;
}

void Transform::operator()(const Point &p, Point *rp) const {
	float x = p.x, y = p.y, z = p.z;
	rp->x = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3];
	rp->y = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3];
	rp->z = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3];
	float wp = m.m[3][0] * x + m.m[3][1] * y + m.m[3][2] * z + m.m[3][3];
	assert(wp != 0.0f);
	if (wp != 1.)
		*rp /= wp;
}

Ray Transform::operator()(const Ray &r) const {
	Ray ray = r;
	(*this)(ray.o, &ray.o);
	(*this)(ray.d, &ray.d);
	return ray;
}

void Transform::operator()(const Ray &r, Ray *rr) const {
	(*this)(r.o, &rr->o);
	(*this)(r.d, &rr->d);
	if (&r != rr) {
		rr->minT = r.minT;
		rr->maxT = r.maxT;
		rr->depth = r.depth;
	}
}

void Transform::operator()(const RayDifferential &r, RayDifferential *rt) const {
    (*this)(Ray(r), (Ray*)rt);
    rt->hasDifferentials = r.hasDifferentials;
    (*this)(r.rxOrigin, &rt->rxOrigin);
    (*this)(r.ryOrigin, &rt->ryOrigin);
    (*this)(r.rxDirection, &rt->rxDirection);
    (*this)(r.ryDirection, &rt->ryDirection);
}



RayDifferential Transform::operator()(const RayDifferential &r) const {
    RayDifferential ret;
    (*this)(Ray(r), (Ray*)&ret);
    ret.hasDifferentials = r.hasDifferentials;
    (*this)(r.rxOrigin, &ret.rxOrigin);
    (*this)(r.ryOrigin, &ret.ryOrigin);
    (*this)(r.rxDirection, &ret.rxDirection);
    (*this)(r.ryDirection, &ret.ryDirection);
    return ret;
}


//对法线变换需要使用转置逆矩阵
Normal Transform::operator()(const Normal& n) const {
	float x = n.x, y = n.y, z = n.z;
	return Normal(invM.m[0][0] * x + invM.m[1][0] * y + invM.m[2][0] * z,
			invM.m[0][1] * x + invM.m[1][1] * y + invM.m[2][1] * z,
			invM.m[0][2] * x + invM.m[1][2] * y + invM.m[2][2] * z);
}

void Transform::operator()(const Normal &n, Normal *normal) const {
	float x = n.x, y = n.y, z = n.z;
	normal->x = invM.m[0][0] * x + invM.m[1][0] * y + invM.m[2][0] * z;
	normal->y = invM.m[0][1] * x + invM.m[1][1] * y + invM.m[2][1] * z;
	normal->z = invM.m[0][2] * x + invM.m[1][2] * y + invM.m[2][2] * z;
}

bool Transform::SwapsHandedness() const {
	float det = ((m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]))
			- (m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]))
			+ (m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0])));
	return det < 0.f;
}

BBox Transform::operator()(const BBox &b) const {
	const Transform &M = *this;
	//不停的做并联操作，来生成新的BBox
	BBox ret(M(Point(b.pMin.x, b.pMin.y, b.pMin.z)));
	ret = Union(ret, M(Point(b.pMax.x, b.pMin.y, b.pMin.z)));
	ret = Union(ret, M(Point(b.pMin.x, b.pMax.y, b.pMin.z)));
	ret = Union(ret, M(Point(b.pMin.x, b.pMin.y, b.pMax.z)));
	ret = Union(ret, M(Point(b.pMin.x, b.pMax.y, b.pMax.z)));
	ret = Union(ret, M(Point(b.pMax.x, b.pMax.y, b.pMin.z)));
	ret = Union(ret, M(Point(b.pMax.x, b.pMin.y, b.pMax.z)));
	ret = Union(ret, M(Point(b.pMax.x, b.pMax.y, b.pMax.z)));
	return ret;
}

//位移转换 参考PBRT
Transform Translate(const Vector &delta) {
	Matrix4X4 m(1, 0, 0, delta.x, 0, 1, 0, delta.y, 0, 0, 1, delta.z, 0, 0, 0,
			1);
	Matrix4X4 minv(1, 0, 0, -delta.x, 0, 1, 0, -delta.y, 0, 0, 1, -delta.z, 0,
			0, 0, 1);
	return Transform(m, minv);
}

//缩放
Transform Scale(float x, float y, float z) {
	Matrix4X4 m(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1);
	Matrix4X4 minv(1.0f / x, 0, 0, 0, 0, 1.0f / y, 0, 0, 0, 0, 1.0f / z, 0, 0,
			0, 0, 1);
	return Transform(m, minv);
}

//旋转矩阵的逆 等于旋转矩阵的转置  所以是正交矩阵
Transform RotateX(float angle) {
	float sinR = sinf(Radians(angle));
	float cosR = cosf(Radians(angle));
	Matrix4X4 m(1, 0, 0, 0, 0, cosR, -sinR, 0, 0, sinR, cosR, 0, 0, 0, 0, 1);
	return Transform(m, Transpose(m));
}

Transform RotateY(float angle) {
	float sinR = sinf(Radians(angle));
	float cosR = cosf(Radians(angle));
	Matrix4X4 m(cosR, 0, sinR, 0, 0, 1, 0, 0, -sinR, 0, cosR, 0, 0, 0, 0, 1);
	return Transform(m, Transpose(m));
}

Transform RotateZ(float angle) {
	float sinR = sinf(Radians(angle));
	float cosR = cosf(Radians(angle));
	Matrix4X4 m(cosR, -sinR, 0, 0, sinR, cosR, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	return Transform(m, Transpose(m));
}

//计算 绕任意轴旋转某角度的方法
/*这个函数拷贝子PBRT代码  建立以axis为z轴的坐标系，计算任意向量旋转后的公式，把原基向量代入公式计算*/
// v`=v_c+v_p*cos(theta)+v_2*sin(theta);
Transform Rotate(float angle, const Vector &axis) {
	Vector a = Normalize(axis);
	float s = sinf(Radians(angle));
	float c = cosf(Radians(angle));
	float m[4][4];

	m[0][0] = a.x * a.x + (1.f - a.x * a.x) * c;
	m[0][1] = a.x * a.y * (1.f - c) - a.z * s;
	m[0][2] = a.x * a.z * (1.f - c) + a.y * s;
	m[0][3] = 0;

	m[1][0] = a.x * a.y * (1.f - c) + a.z * s;
	m[1][1] = a.y * a.y + (1.f - a.y * a.y) * c;
	m[1][2] = a.y * a.z * (1.f - c) - a.x * s;
	m[1][3] = 0;

	m[2][0] = a.x * a.z * (1.f - c) - a.y * s;
	m[2][1] = a.y * a.z * (1.f - c) + a.x * s;
	m[2][2] = a.z * a.z + (1.f - a.z * a.z) * c;
	m[2][3] = 0;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;

	Matrix4X4 mat = Matrix4X4(m);
	return Transform(mat, Transpose(mat));
}

Transform Transform::operator*(const Transform& tran) const {
	Matrix4X4 m = Matrix4X4::Mul(this->m, tran.m);
	Matrix4X4 mInv = Matrix4X4::Mul(tran.invM, this->invM);
	return Transform(m, mInv);
}

Transform Orthographic(float znear, float zfar) {
	return Scale(1.f, 1.f, 1.f / (zfar - znear))
			* Translate(Vector(0.f, 0.f, -znear));
}

Transform Perspective(float fov, float n, float f){
	 Matrix4X4 persp = Matrix4X4(1, 0,           0,              0,
	                                0, 1,           0,              0,
	                                0, 0, f / (f - n), -f*n / (f - n),
	                                0, 0,           1,              0);
	    //使用fov来缩放到标准空间
	    float invTanAng = 1.f / tanf(Radians(fov) / 2.f);
	    return Scale(invTanAng, invTanAng, 1) * Transform(persp);
}
