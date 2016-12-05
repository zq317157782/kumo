//
// Created by 诸谦 on 16/6/10.
//

#ifndef RAYTRACER_GEOMETRY_H
#define RAYTRACER_GEOMETRY_H

#include <kumo.h>

//空间向量类

class Vector {
//field
public:
	Float x, y, z;

//function
public:
	Vector() {
		x = y = z = 0.f;
	}
	Vector(Float xx, Float yy, Float zz) :
			x(xx), y(yy), z(zz) {

	}

	Vector(const Vector& v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	Vector &operator=(const Vector &v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	//加法
	Vector operator +(const Vector& v) const {
		return Vector(x + v.x, y + v.y, z + v.z);
	}

	Vector& operator+=(const Vector& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	//减法
	Vector operator-(const Vector& v) const {
		return Vector(x - v.x, y - v.y, z - v.z);
	}

	Vector& operator-=(const Vector& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	//取反
	Vector operator-() const {
		return Vector(-x, -y, -z);
	}
	//乘法
	Vector operator*(Float f) const {
		return Vector(x * f, y * f, z * f);
	}

	Vector& operator*=(Float f) {
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	//除法
	Vector operator/(Float f) const {
		assert(f != 0);
		Float inv = 1 / f;
		return Vector(x * inv, y * inv, z * inv);
	}

	Vector& operator/=(Float f) {
		assert(f != 0);
		Float inv = 1 / f;
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}

	//平方长度
	Float LengthSqr() const {
		return x * x + y * y + z * z;
	}
	//长度 or 模
	Float Length() const {
		return sqrtf(LengthSqr());
	}

	bool operator==(const Vector& v) const {
		if (x == v.x && y == v.y && z == v.z) {
			return true;
		}
		return false;
	}
	bool operator!=(const Vector& v) const {
		if (x != v.x || y != v.y || z != v.z) {
			return true;
		}
		return false;
	}

	//专门给取操作用
	Float operator[](int i) const {
		assert(i >= 0 && i <= 2);
		return (&x)[i];
	}
	//专门给赋值操作
	Float& operator[](int i) {
		assert(i >= 0 && i <= 2);
		return (&x)[i];
	}

	//显示Point到Vector的转换
	explicit Vector(const Point &p);
	explicit Vector(const Normal &n);

};

inline Float Dot(const Vector& v1, const Vector& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline Float AbsDot(const Vector& v1, const Vector& v2) {
	return fabsf(Dot(v1, v2));
}

//基于左手坐标系
inline Vector Cross(const Vector& v1, const Vector& v2) {

	return Vector(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
}

inline Vector Normalize(const Vector& v) {
	return v / v.Length();
}

inline Vector operator*(Float f, const Vector& v) {
	return v * f;
}

class Point {

public:
	Float x, y, z;
public:
	Point() {
		x = y = z = 0.0f;
	}

	Point(Float xx, Float yy, Float zz) :
			x(xx), y(yy), z(zz) {

	}

	Point(const Point& p) {
		x = p.x;
		y = p.y;
		z = p.z;
	}

	Point& operator=(const Point& p) {
		x = p.x;
		y = p.y;
		z = p.z;
		return *this;
	}

	//加法
	Point operator+(const Point& p) const {
		return Point(x + p.x, y + p.y, z + p.z);
	}

	Point& operator+=(const Point& p) {
		x += p.x;
		y += p.y;
		z += p.z;
		return *this;
	}

	//减法
	Vector operator-(const Point& p) const {
		return Vector(x - p.x, y - p.y, z - p.z);
	}

	//乘法
	Point operator*(Float f) const {
		return Point(x * f, y * f, z * f);
	}

	Point& operator *=(Float f) {
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	//除法
	Point operator/(Float f) const {
		assert(f != 0);
		Float inv = 1.0f / f;
		return Point(x * inv, y * inv, z * inv);
	}
	Point& operator/=(Float f) {
		assert(f != 0);
		Float inv = 1.0f / f;
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}
	//取反
	Point operator-() const {
		return Point(-x, -y, -z);
	}

	//和Vector相关的操作
	Point operator+(const Vector& v) const {
		return Point(x + v.x, y + v.y, z + v.z);
	}
	Point& operator+=(const Vector& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	Point operator-(const Vector& v) const {
		return Point(x - v.x, y - v.y, z - v.z);
	}
	Point& operator-=(const Vector& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	bool operator==(const Point& p) const {
		if (x == p.x && y == p.y && z == p.z) {
			return true;
		}
		return false;
	}

	bool operator!=(const Point& p) const {
		if (x != p.x || y != p.y || z != p.z) {
			return true;
		}
		return false;
	}

	Float operator[](int i) const {
		assert(i >= 0 && i <= 2);
		return (&x)[i];
	}
	Float& operator[](int i) {
		assert(i >= 0 && i <= 2);
		return (&x)[i];
	}
};

inline Vector::Vector(const Point &p) :
		x(p.x), y(p.y), z(p.z) {
}

inline Float Distance(const Point& p1, const Point &p2) {
	return (p1 - p2).Length();
}

inline Float DistanceSqr(const Point& p1, const Point &p2) {
	return (p1 - p2).LengthSqr();
}
inline Point operator*(Float f, const Point& p) {
	return p * f;
}

//法线
class Normal {
public:
	Float x, y, z;
public:
	Normal() {
		x = y = z = 0;
	}
	Normal(Float xx, Float yy, Float zz) :
			x(xx), y(yy), z(zz) {
	}
	Normal(const Normal& n) :
			x(n.x), y(n.y), z(n.z) {

	}
	Normal& operator=(const Normal& n) {
		x = n.x;
		y = n.y;
		z = n.z;
		return *this;
	}

	Normal operator-() const {
		return Normal(-x, -y, -z);
	}
	Normal operator+(const Normal& n) const {
		return Normal(x + n.x, y + n.y, z + n.z);
	}
	Normal& operator+=(const Normal& n) {
		x += n.x;
		y += n.y;
		z += n.z;
		return *this;
	}
	Normal operator-(const Normal& n) const {
		return Normal(x - n.x, y - n.y, z - n.z);
	}
	Normal operator-=(const Normal& n) {
		x -= n.x;
		y -= n.y;
		z -= n.z;
		return *this;
	}

	Normal operator*(Float f) const {
		return Normal(x * f, y * f, z * f);
	}

	Normal& operator*=(Float f) {
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	Normal operator/(Float f) const {
		assert(f != 0);
		Float inv = 1.0f / f;
		return Normal(x * inv, y * inv, z * inv);
	}

	Normal& operator/=(Float f) {
		assert(f != 0);
		Float inv = 1.0f / f;
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}

	Float operator[](int i) const {
		assert(i >= 0 && i <= 2);
		return (&x)[i];
	}

	Float& operator[](int i) {
		assert(i >= 0 && i <= 2);
		return (&x)[i];
	}

	bool operator==(const Normal& n) const {
		if (x == n.x && y == n.y && z == n.z) {
			return true;
		}
		return false;
	}

	bool operator!=(const Normal& n) const {
		if (x != n.x || y != n.y || z != n.z) {
			return true;
		}
		return false;
	}
	//平方长度
	Float LengthSqr() const {
		return x * x + y * y + z * z;
	}
	//长度 or 模
	Float Length() const {
		return sqrtf(LengthSqr());
	}

	explicit Normal(const Vector& v) :
			x(v.x), y(v.y), z(v.z) {
	}
	;
};

inline Vector::Vector(const Normal &n) :
		x(n.x), y(n.y), z(n.z) {

}

inline Normal operator*(Float f, const Normal& n) {
	return n * f;
}

inline Float Dot(const Vector& v1, const Normal& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline Float Dot(const Normal& v1, const Normal& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline Float Dot(const Normal& v1, const Vector& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Float AbsDot(const Vector& v1, const Normal& v2) {
	return fabsf(Dot(v1, v2));
}
inline Float AbsDot(const Normal& v1, const Normal& v2) {
	return fabsf(Dot(v1, v2));
}
inline Float AbsDot(const Normal& v1, const Vector& v2) {
	return fabsf(Dot(v1, v2));
}

//基于左手坐标系
inline Vector Cross(const Vector& v1, const Normal& v2) {

	return Vector(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
}
//基于左手坐标系
inline Vector Cross(const Normal& v1, const Vector& v2) {

	return Vector(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
}
//基于左手坐标系
inline Vector Cross(const Normal& v1, const Normal& v2) {

	return Vector(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
}

inline Normal Normalize(const Normal& n) {
	return n / n.Length();
}

//根据第二个参数 返回和第二个参数在同一半球中的向量火法线
inline Normal Faceforward(const Normal &n, const Vector &v) {
	return (Dot(n, v) < 0.f) ? -n : n;
}
inline Normal Faceforward(const Normal &n, const Normal &n2) {
	return (Dot(n, n2) < 0.f) ? -n : n;
}
inline Vector Faceforward(const Vector &v, const Vector &v2) {
	return (Dot(v, v2) < 0.f) ? -v : v;
}
inline Vector Faceforward(const Vector &v, const Normal &n2) {
	return (Dot(v, n2) < 0.f) ? -v : v;
}

//根据一个向量生成一个坐标系
inline void CoordinateSystem(const Vector &v1, Vector *v2, Vector *v3) {
	if (fabsf(v1.x) > fabsf(v1.y)) {
		Float invLen = 1.f / sqrtf(v1.x * v1.x + v1.z * v1.z);
		*v2 = Vector(-v1.z * invLen, 0.f, v1.x * invLen);
	} else {
		Float invLen = 1.f / sqrtf(v1.y * v1.y + v1.z * v1.z);
		*v2 = Vector(0.f, v1.z * invLen, -v1.y * invLen);
	}
	*v3 = Cross(v1, *v2);
}

class Ray {
public:
	Point o;
	Vector d;
	mutable Float minT, maxT;
	int depth;    //射线迭代的深度

public:
	Ray(const Point& oo, const Vector& dd, Float start, Float end = INFINITY,
			int depth = 0) :
			o(oo), d(dd), minT(start), maxT(end), depth(depth) {
	}
	Ray(const Point &oo, const Vector &dd, const Ray &parent, Float start,
			Float end = INFINITY) :
			o(oo), d(dd), minT(start), maxT(end), depth(parent.depth + 1) {
	}
	Ray() {
	}
	Point operator()(Float t) const {
		return o + d * t;
	}
};

//带微分信息的射线,其实就是在X和Y方向上有相隔1像素的两条另外的射线
class RayDifferential: public Ray {
public:
	bool hasDifferentials;
	Point rxOrigin, ryOrigin;
	Vector rxDirection, ryDirection;

	RayDifferential() {
		hasDifferentials = false;
	}

	RayDifferential(const Point& oo, const Vector& dd, Float start, Float end =
	INFINITY, int depth = 0) :
			Ray(oo, dd, start, end, depth) {
		hasDifferentials = false;
	}
	RayDifferential(const Point &oo, const Vector &dd, const Ray &parent,
			Float start, Float end = INFINITY) :
			Ray(oo, dd, start, end, parent.depth + 1) {
		hasDifferentials = false;
	}

	RayDifferential(const Ray& r) :
			Ray(r) {
		hasDifferentials = false;
	}
	;

	//缩放
	void ScaleDifferentials(Float s) {
		rxOrigin = o + (rxOrigin - o) * s;
		ryOrigin = o + (ryOrigin - o) * s;
		rxDirection = d + (rxDirection - d) * s;
		ryDirection = d + (ryDirection - d) * s;
	}

};

//球坐标到向量的变换
inline Vector SphericalDirection(Float sintheta, Float costheta, Float phi) {
	return Vector(sintheta * cosf(phi), sintheta * sinf(phi), costheta);
}

//bounding-box
class BBox {
public:
	Point pMin;
	Point pMax;
	BBox() {
		pMin = Point(INFINITY, INFINITY, INFINITY);
		pMax = Point(-INFINITY, -INFINITY, -INFINITY);
	}

	BBox(const Point& p) :
			pMin(p), pMax(p) {

	}

	BBox(const Point& p1, const Point& p2) {
		pMin = Point(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
				std::min(p1.z, p2.z));
		pMax = Point(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
				std::max(p1.z, p2.z));
	}
	bool Overlaps(const BBox &b) const {
		bool x = (pMax.x >= b.pMin.x) && (pMin.x <= b.pMax.x);
		bool y = (pMax.y >= b.pMin.y) && (pMin.y <= b.pMax.y);
		bool z = (pMax.z >= b.pMin.z) && (pMin.z <= b.pMax.z);
		return (x && y && z);
	}
	bool Inside(const Point &pt) const {
		return (pt.x >= pMin.x && pt.x <= pMax.x && pt.y >= pMin.y
				&& pt.y <= pMax.y && pt.z >= pMin.z && pt.z <= pMax.z);
	}
	void Expand(Float delta) {
		pMin -= Vector(delta, delta, delta);
		pMax += Vector(delta, delta, delta);
	}
	Float SurfaceArea() const {
		Vector d = pMax - pMin;
		return 2.f * (d.x * d.y + d.x * d.z + d.y * d.z);
	}
	Float Volume() const {
		Vector d = pMax - pMin;
		return d.x * d.y * d.z;
	}
	bool IntersectP(const Ray &ray, Float *hitt0 = nullptr, Float *hitt1 =
			nullptr) const;

	int MaximumExtent() const {
		Vector diag = pMax - pMin;
		if (diag.x > diag.y && diag.x > diag.z)
			return 0;
		else if (diag.y > diag.z)
			return 1;
		else
			return 2;
	}

	const Point &operator[](int i) const {
		return (&pMin)[i];
	}
	Point &operator[](int i) {
		return (&pMin)[i];
	}
};

BBox Union(const BBox &b, const Point &p);

BBox Union(const BBox &b, const BBox &b2);

#endif //RAYTRACER_GEOMETRY_H
