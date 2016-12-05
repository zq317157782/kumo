//
// Created by 诸谦 on 16/6/10.
//

#ifndef RAYTRACER_GEOMETRY_H
#define RAYTRACER_GEOMETRY_H

#include <kumo.h>

//判断是否有NaN的值出现
template<typename T>
inline bool IsNaN(T n) {
	//return !(n==n);//使用自己和自己比较的方式 如果一个数是NaN的话，它和任意数比较都为false,但是编译器可能会进行优化，导致失效
	return isnan(n); //这是使用标准库的判断方法
}

//三维向量
template<typename T>
class Vector3 {
public:
	Float x, y, z;
public:
	Vector3() {
		x = y = z = 0;
	}
	Vector3(T xx, T yy, T zz) :
			x(xx), y(yy), z(zz) {
		//只有为每个分量单独赋值的时候才需要下NaN的断言
		Assert(!HasNaNs());
	}

	explicit Vector3(const Point3<T>& p) {
		Assert(!p.HasNaNs());
		x = p.x;
		y = p.y;
		z = p.z;
	}

	explicit Vector3(const Normal3<T>& n) {
		Assert(!n.HasNaNs());
		x = n.x;
		y = n.y;
		z = n.z;
	}
//这里默认的赋值函数和复制函数都不错，所以只在DEBUG模式下才需要自己定义，并且下断言来调试
#ifdef DEBUG_BUILD
	Vector3(const Vector3<T>& v) {
		Assert(!v.HasNaNs());
		x = v.x;
		y = v.y;
		z = v.z;
	}

	Vector3<T>& operator=(const Vector3<T>& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
#endif
	Vector3<T> operator+(const Vector3<T>& v) const {
		Assert(!v.HasNaNs());
		return Vector3<T>(x + v.x, y + v.y, z + v.z);
	}

	Vector3<T>& operator+=(const Vector3<T>& v) {
		Assert(!v.HasNaNs());
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3<T> operator-(const Vector3<T>& v) const {
		Assert(!v.HasNaNs());
		return Vector3<T>(x - v.x, y - v.y, z - v.z);
	}

	Vector3<T>& operator-=(const Vector3<T>& v) {
		Assert(!v.HasNaNs());
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	template<typename U>
	Vector3<T> operator*(U n) const {
		Assert(!IsNaN(n));
		return Vector3<T>(x * n, y * n, z * n);
	}

	template<typename U>
	Vector3<T>& operator*=(U n) {
		Assert(!IsNaN(n));
		x *= n;
		y *= n;
		z *= n;
		return *this;
	}

	template<typename U>
	Vector3<T> operator/(U n) const {
		Assert(!IsNaN(n));
		Assert(n != 0);
		T f = (Float) 1 / n;
		return Vector3<T>(f * x, f * y, f * z);
	}

	template<typename U>
	Vector3<T>& operator/=(U n) {
		Assert(!IsNaN(n));
		Assert(n != 0);
		T f = (Float) 1 / n;
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	Vector3<T> operator-() const {
		return Vector3<T>(-x, -y, -z);
	}

	bool operator==(const Vector3<T>& v) const {
		if (x == v.x && y == v.y && z == v.z)
			return true;
		return false;
	}

	bool operator!=(const Vector3<T>& v) const {
		if (x != v.x || y != v.y || z != v.z)
			return true;
		return false;
	}

	//返回向量的数量级的平方
	T MagnitudeSquared() const {
		return x * x + y * y + z * z;
	}

	T LengthSquared() const {
		return x * x + y * y + z * z;
	}

	//返回向量的数量级 有开根操作
	T Magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}

	T Length() const {
		return sqrt(x * x + y * y + z * z);
	}

	T operator[](int index) const {
		Assert(index >= 0 && index < 3);
		return (&x)[index];
	}

	T& operator[](int index) {
		Assert(index >= 0 && index < 3);
		return (&x)[index];
	}

	//重构ostream方法
	friend std::ostream &operator<<(std::ostream &os, const Vector3<T> &v) {
		os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
		return os;
	}
	//判断三个分量中有没有NaN的变量
	bool HasNaNs() const {
		return ::IsNaN(x) || ::IsNaN(y) || ::IsNaN(z);
	}
};

typedef Vector3<Float> Vector3f;
typedef Vector3<int> Vector3i;

//二维向量
template<typename T>
class Vector2 {
public:
	Float x, y;
public:
	Vector2() {
		x = y = 0;
	}
	Vector2(T xx, T yy) :
			x(xx), y(yy) {
		//只有为每个分量单独赋值的时候才需要下NaN的断言
		Assert(!HasNaNs());
	}

	explicit Vector2(const Point2<T> p) {
		Assert(!p.HasNaNs());
		x = p.x;
		y = p.y;
	}
//这里默认的赋值函数和复制函数都不错，所以只在DEBUG模式下才需要自己定义，并且下断言来调试
#ifdef DEBUG_BUILD
	Vector2(const Vector2<T>& v) {
		Assert(!v.HasNaNs());
		x = v.x;
		y = v.y;
	}
	Vector2<T>& operator=(const Vector2<T>& v) {
		Assert(v.HasNaNs());
		x = v.x;
		y = v.y;
		return *this;
	}
#endif
	Vector2<T> operator+(const Vector2<T>& v) const {
		Assert(!v.HasNaNs());
		return Vector2<T>(x + v.x, y + v.y);
	}

	Vector2<T>& operator+=(const Vector2<T>& v) {
		Assert(!v.HasNaNs());
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector2<T> operator-(const Vector2<T>& v) const {
		Assert(!v.HasNaNs());
		return Vector2<T>(x - v.x, y - v.y);
	}

	Vector2<T>& operator-=(const Vector2<T>& v) {
		Assert(!v.HasNaNs());
		x -= v.x;
		y -= v.y;
		return *this;
	}

	template<typename U>
	Vector2<T> operator*(U n) const {
		Assert(!IsNaN(n));
		return Vector2<T>(x * n, y * n);
	}

	template<typename U>
	Vector2<T>& operator*=(U n) {
		Assert(!IsNaN(n));
		x *= n;
		y *= n;
		return *this;
	}

	template<typename U>
	Vector2<T> operator/(U n) const {
		Assert(!IsNaN(n));
		Assert(n != 0);
		T f = (Float) 1 / n;
		return Vector2<T>(f * x, f * y);
	}

	template<typename U>
	Vector2<T>& operator/=(U n) {
		Assert(!IsNaN(n));
		Assert(n != 0);
		T f = (Float) 1 / n;
		x *= f;
		y *= f;
		return *this;
	}

	Vector2<T> operator-() const {
		return Vector2<T>(-x, -y);
	}

	bool operator==(const Vector2<T>& v) const {
		if (x == v.x && y == v.y)
			return true;
		return false;
	}

	bool operator!=(const Vector2<T>& v) const {
		if (x != v.x || y != v.y)
			return true;
		return false;
	}

	//返回向量的数量级的平方
	T MagnitudeSquared() const {
		return x * x + y * y;
	}

	T LengthSquared() const {
		return MagnitudeSquared();
	}

	//返回向量的数量级 有开根操作
	T Magnitude() const {
		return sqrt(x * x + y * y);
	}

	T Length() const {
		return Magnitude();
	}

	T operator[](int index) const {
		Assert(index >= 0 && index < 2);
		return (&x)[index];
	}

	T& operator[](int index) {
		Assert(index >= 0 && index < 2);
		return (&x)[index];
	}

	//重构ostream方法
	friend std::ostream &operator<<(std::ostream &os, const Vector2<T> &v) {
		os << "[" << v.x << ", " << v.y << "]";
		return os;
	}
	//判断分量中有没有NaN的变量
	bool HasNaNs() const {
		return ::IsNaN(x) || ::IsNaN(y);
	}
};
typedef Vector2<Float> Vector2f;
typedef Vector2<int> Vector2i;

template<typename T>
class Point3 {
public:
	T x, y, z;
public:
	Point3() {
		x = y = z = 0;
	}

	Point3(T xx, T yy, T zz) :
			x(xx), y(yy), z(zz) {
		Assert(!HasNaNs());
	}

//这里默认的赋值函数和复制函数都不错，所以只在DEBUG模式下才需要自己定义，并且下断言来调试
#ifdef DEBUG_BUILD
	Point3(const Point3<T>& p) {
		Assert(!p.HasNaNs());
		x = p.x;
		y = p.y;
		z = p.z;

	}
	Point3<T>& operator=(const Point3<T>& p) {
		Assert(!p.HasNaNs());
		x = p.x;
		y = p.y;
		z = p.z;
		return *this;
	}
#endif

	Point3<T> operator+(const Point3<T>& p) const {
		Assert(!p.HasNaNs());
		return Point3<T>(x + p.x, y + p.y, z + p.z);
	}

	Point3<T>& operator+=(const Point3<T>& p) {
		Assert(!p.HasNaNs());
		x += p.x;
		y += p.y;
		z += p.z;
		return *this;
	}

	//两个空间点相减 返回的是一个空间向量
	Vector3<T> operator-(const Point3<T>& p) const {
		Assert(!p.HasNaNs());
		return Vector3<T>(x - p.x, y - p.y, z - p.z);
	}

	template<typename U>
	Point3<T> operator*(U u) const {
		Assert(!IsNaN(u));
		return Point3<T>(x * u, y * u, z * u);
	}

	template<typename U>
	Point3<T>& operator*=(U u) {
		Assert(!IsNaN(u));
		x *= u;
		y *= u;
		z *= u;
		return *this;
	}

	template<typename U>
	Point3<T> operator/(U u) const {
		Assert(!IsNaN(u));
		Assert(u != 0);
		Float f = (Float) 1 / u;
		return Point3<T>(x * f, y * f, z * f);
	}

	template<typename U>
	Point3<T>& operator/=(U u) {
		Assert(!IsNaN(u));
		Assert(u != 0);
		Float f = (Float) 1 / u;
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	Point3<T> operator-() const {
		return Point3<T>(-x, -y, -z);
	}

	//和Vector相关的操作
	Point3<T> operator+(const Vector3<T>& v) const {
		Assert(!v.HasNaNs());
		return Point3<T>(x + v.x, y + v.y, z + v.z);
	}
	Point3<T>& operator+=(const Vector3<T>& v) {
		Assert(!v.HasNaNs());
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	bool operator==(const Point3<T>& p) const {
		if (p.x == x && p.y == y && p.z == z)
			return true;
		return false;
	}

	bool operator!=(const Point3<T>& p) const {
		if (p.x != x || p.y != y || p.z != z)
			return true;
		return false;
	}

	T operator[](int index) const {
		Assert(index >= 0 && index < 3);
		return (&x)[index];
	}
	T& operator[](int index) {
		Assert(index >= 0 && index < 3);
		return (&x)[index];
	}

	//重构ostream方法
	friend std::ostream &operator<<(std::ostream &os, const Point3<T> &v) {
		os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
		return os;
	}

	//判断分量中有没有NaN的变量
	bool HasNaNs() const {
		return ::IsNaN(x) || ::IsNaN(y);
	}
};

typedef Point3<Float> Point3f;
typedef Point3<int> Point3i;

template<typename T>
class Point2 {
public:
	T x, y;
public:
	Point2() {
		x = y = 0;
	}

	Point2(T xx, T yy) :
			x(xx), y(yy) {
		Assert(!HasNaNs());
	}
	//这里默认的赋值函数和复制函数都不错，所以只在DEBUG模式下才需要自己定义，并且下断言来调试
#ifdef DEBUG_BUILD
	Point2(const Point2<T>& p) {
		Assert(!p.HasNaNs());
		x = p.x;
		y = p.y;
	}
	Point2<T>& operator=(const Point2<T>& p) {
		Assert(!p.HasNaNs());
		x = p.x;
		y = p.y;
		return *this;
	}
#endif
	Point2<T> operator+(const Point2<T>& p) const {
		Assert(!p.HasNaNs());
		return Point2<T>(x + p.x, y + p.y);
	}

	Point2<T>& operator+=(const Point2<T>& p) {
		Assert(!p.HasNaNs());
		x += p.x;
		y += p.y;
		return *this;
	}

	Vector2<T> operator-(const Point2<T>& p) const {
		Assert(!p.HasNaNs());
		return Vector2<T>(x - p.x, y - p.y);
	}

	template<typename U>
	Point2<T> operator*(U u) const {
		Assert(!IsNaN(u));
		return Point2<T>(x * u, y * u);
	}

	template<typename U>
	Point2<T>& operator*=(U u) {
		Assert(!IsNaN(u));
		x *= u;
		y *= u;
		return *this;
	}

	template<typename U>
	Point2<T> operator/(U u) const {
		Assert(!IsNaN(u));
		Assert(u != 0);
		Float f = (Float) 1 / u;
		return Point2<T>(x * f, y * f);
	}

	template<typename U>
	Point2<T>& operator/=(U u) {
		Assert(!IsNaN(u));
		Assert(u != 0);
		Float f = (Float) 1 / u;
		x *= f;
		y *= f;
		return *this;
	}

	Point2<T> operator-() const {
		return Point2<T>(-x, -y);
	}

	//和Vector相关的操作
	Point2<T> operator+(const Vector3<T>& v) const {
		Assert(!v.HasNaNs());
		return Point2<T>(x + v.x, y + v.y);
	}
	Point2<T>& operator+=(const Vector3<T>& v) {
		Assert(!v.HasNaNs());
		x += v.x;
		y += v.y;
		return *this;
	}

	bool operator==(const Point2<T>& p) const {
		if (p.x == x && p.y == y)
			return true;
		return false;
	}

	bool operator!=(const Point2<T>& p) const {
		if (p.x != x || p.y != y)
			return true;
		return false;
	}

	T operator[](int index) const {
		Assert(index >= 0 && index < 3);
		return (&x)[index];
	}
	T& operator[](int index) {
		Assert(index >= 0 && index < 3);
		return (&x)[index];
	}

	//重构ostream方法
	friend std::ostream &operator<<(std::ostream &os, const Point2<T> &v) {
		os << "[" << v.x << ", " << v.y << "]";
		return os;
	}
	//判断分量中有没有NaN的变量
	bool HasNaNs() const {
		return ::IsNaN(x) || ::IsNaN(y);
	}
};

typedef Point2<Float> Point2f;
typedef Point2<int> Point2i;

//三分量法线
template<typename T>
class Normal3 {
public:
	T x, y, z;
public:
	Normal3() :
			x(0), y(0), z(0) {
	}

	Normal3(T xx, T yy, T zz) :
			x(xx), y(yy), z(zz) {
		Assert(!HasNaNs());
	}
	//这里默认的赋值函数和复制函数都不错，所以只在DEBUG模式下才需要自己定义，并且下断言来调试
#ifdef DEBUG_BUILD
	Normal3(const Normal3& nl) {
		Assert(!nl.HasNaNs());
		x = nl.x;
		y = nl.y;
		z = nl.z;
	}
	Normal3<T>& operator=(const Normal3& nl) {
		Assert(!nl.HasNaNs());
		x = nl.x;
		y = nl.y;
		z = nl.z;
		return *this;
	}
#endif
	Normal3<T> operator-() const {
		return Normal3<T>(-x, -y, -z);
	}
	Normal3<T> operator+(const Normal3<T>& nl) const {
		Assert(!nl.HasNaNs());
		return Normal3<T>(x + nl.x, y + nl.y, z + nl.z);
	}

	Normal3<T>& operator+=(const Normal3<T>& nl) {
		Assert(!nl.HasNaNs());
		x += nl.x;
		y += nl.y;
		z += nl.z;
		return *this;
	}
	Normal3<T> operator-(const Normal3<T>& n) const {
		Assert(!n.HasNaNs());
		return Normal3<T>(x - n.x, y - n.y, z - n.z);
	}

	Normal3<T> operator-=(const Normal3<T>& n) {
		Assert(!n.HasNaNs());
		x -= n.x;
		y -= n.y;
		z -= n.z;
		return *this;
	}

	template<typename U>
	Normal3<T> operator*(U n) const {
		Assert(!IsNaN(n));
		return Normal3<T>(x * n, y * n, z * n);
	}

	template<typename U>
	Normal3<T>& operator*=(U f) {
		Assert(!IsNaN(f));
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}
	template<typename U>
	Normal3<T> operator/(U f) const {
		Assert(!IsNaN(f));
		Assert(f != 0);
		float inv = 1.0f / f;
		return Normal3<T>(x * inv, y * inv, z * inv);
	}

	template<typename U>
	Normal3<T>& operator/=(U f) {
		Assert(!IsNaN(f));
		Assert(f != 0);
		float inv = 1.0f / f;
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}

	T operator[](int i) const {
		Assert(i >= 0 && i <= 2);
		return (&x)[i];
	}

	T& operator[](int i) {
		Assert(i >= 0 && i <= 2);
		return (&x)[i];
	}

	bool operator==(const Normal3<T>& n) const {
		Assert(!n.HasNaNs());
		if (x == n.x && y == n.y && z == n.z) {
			return true;
		}
		return false;
	}

	bool operator!=(const Normal3<T>& n) const {
		Assert(!n.HasNaNs());
		if (x != n.x || y != n.y || z != n.z) {
			return true;
		}
		return false;
	}
	T LengthSquared() const {
		return x * x + y * y + z * z;
	}
	T Length() const {
		return sqrt(x * x + y * y + z * z);
	}

	explicit Normal3<T>(const Vector3<T>& v) :
			x(v.x), y(v.y), z(v.z) {
		Assert(!HasNaNs());
	}

	//判断分量中有没有NaN的变量
	bool HasNaNs() const {
		return ::IsNaN(x) || ::IsNaN(y) || ::IsNaN(z);
	}

	//重构ostream方法
	friend std::ostream &operator<<(std::ostream &os, const Normal3<T> &n) {
		os << "[" << n.x << ", " << n.y << ", " << n.z << "]";
		return os;
	}
};

typedef Normal3<Float> Normal3f;

//AABB盒
template<typename T>
class Bound3 {
public:
//AABB盒的最小顶点和最大顶点
	Point3<T> minPoint, maxPoint;
public:
	Bound3() {
		//默认构造函数最小点取最大值，最大点取最小值
		//PBRT_V2中是取了float的两个无限值
		T minValue = std::numeric_limits<T>::lowest();//lowest是带符号最小的浮点数 min是不带符号最小的浮点数，不包括0
		T maxValue = std::numeric_limits<T>::max();
		minPoint = Point3<T>(maxValue, maxValue, maxValue);
		maxPoint = Point3<T>(minValue, minValue, minValue);
	}
	Bound3(const Point3<T>& p) :
			minPoint(p), maxPoint(p) {
	}
	Bound3(const Point3<T>& p1, const Point3<T>& p2) :
			minPoint(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
					std::min(p1.z, p2.z)), maxPoint(std::max(p1.x, p2.x),
					std::max(p1.y, p2.y), std::max(p1.z, p2.z)) {
	}
	//这里通过索引来访问minPoint和maxPoint属性
	//这里用const ref提高访问class对象的速度，但是同时不能修改值，因为是引用
	const Point3<T>& operator[](int index) const {
		Assert(index >= 0 && index < 3);
		return (&minPoint)[index];
	}

	Point3<T>& operator[](int index) {
		Assert(index >= 0 && index < 3);
		return (&minPoint)[index];
	}

	//顺序0~7: 前：左下，右下，左上，右上，后：左下，右下，左上，右上
	Point3<T> Corner(int index) const {
		Assert(index >= 0 && index < 8);
		T x = (*this)[index & 1].x;	//偶数取minPoint.x 奇数取maxPoint.x;
		T y = (*this)[index & 2 ? 1 : 0].y;	//index第二位是0取minPoint.y,否则取maxPoint.y;
		T z = (*this)[index & 4 ? 1 : 0].z;	//类推z
		return Point3<T>(x, y, z);
	}

	//返回对角线向量
	Vector3<T> Diagonal() const {
		return (maxPoint - minPoint);
	}

	//求面积
	T SurfaceArea() const {
		Vector3<T> d = Diagonal();
		return (d.x * d.y + d.x * d.z + d.y * d.z) * 2.0f;
	}

	//求体积
	T Volume() const {
		Vector3<T> d = Diagonal();
		return d.x * d.y * d.z;
	}

	//获取最大的边界
	int MaximumExtent() const {
		Vector3<T> diag = Diagonal();
		if (diag.x > diag.y && diag.x > diag.z)
			return 0;
		else if (diag.y > diag.z)
			return 1;
		else
			return 2;
	}

	bool operator==(const Bound3<T>& b) const {
		if (minPoint == b.minPoint && maxPoint == b.maxPoint) {
			return true;
		}
		return false;
	}

	bool operator!=(const Bound3<T>& b) const {
		if (minPoint != b.minPoint || maxPoint != b.maxPoint) {
			return true;
		}
		return false;
	}
	//重构ostream方法
	friend std::ostream &operator<<(std::ostream &os, const Bound3<T> &n) {
		os << "[" << n.minPoint << " , " << n.maxPoint << "]";
		return os;
	}

};

typedef Bound3<Float> Bound3f;
typedef Bound3<int> Bound3i;

//平面Rect
template<typename T>
class Bound2 {
public:
	Point2<T> minPoint, maxPoint;
public:
	Bound2() {
		//默认构造函数最小点取最大值，最大点取最小值
		//PBRT_V2中是取了float的两个无限值
		T minValue = std::numeric_limits<T>::lowest();//lowest是带符号最小的浮点数 min是不带符号最小的浮点数，不包括0
		T maxValue = std::numeric_limits<T>::max();
		minPoint = Point2<T>(maxValue, maxValue);
		maxPoint = Point2<T>(minValue, minValue);
	}
	Bound2(const Point2<T>& p) :
			minPoint(p), maxPoint(p) {
	}
	Bound2(const Point2<T>& p1, const Point2<T>& p2) :
			minPoint(std::min(p1.x, p2.x), std::min(p1.y, p2.y)), maxPoint(
					std::max(p1.x, p2.x), std::max(p1.y, p2.y)) {
	}

//	Bound2<T>& operator=(const Bound2<T>& b){
//		minPoint=b.minPoint;
//		maxPoint=b.maxPoint;
//		return *this;
//	}

	//这里通过索引来访问minPoint和maxPoint属性
	//这里用const ref提高访问class对象的速度，但是同时不能修改值，因为是引用
	const Point2<T>& operator[](int index) const {
		Assert(index >= 0 && index < 2);
		return (&minPoint)[index];
	}

	Point2<T>& operator[](int index) {
		Assert(index >= 0 && index < 2);
		return (&minPoint)[index];
	}

	Point2<T> Corner(int index) const {
		Assert(index >= 0 && index < 4);
		T x = (*this)[index & 1].x;	//偶数取minPoint.x 奇数取maxPoint.x;
		T y = (*this)[index & 2 ? 1 : 0].y;	//index第二位是0取minPoint.y,否则取maxPoint.y;
		return Point2<T>(x, y);
	}

	//返回对角线向量
	Vector2<T> Diagonal() const {
		return (maxPoint - minPoint);
	}

	//求面积
	T SurfaceArea() const {
		Vector2<T> d = Diagonal();
		return d.x * d.y;
	}

	//获取最大的边界
	int MaximumExtent() const {
		Vector2<T> diag = Diagonal();
		if (diag.x > diag.y)
			return 0;
		else
			return 1;
	}

	bool operator==(const Bound2<T>& b) const {
		if (minPoint == b.minPoint && maxPoint == b.maxPoint) {
			return true;
		}
		return false;
	}

	bool operator!=(const Bound2<T>& b) const {
		if (minPoint != b.minPoint || maxPoint != b.maxPoint) {
			return true;
		}
		return false;
	}

	//重构ostream方法
	friend std::ostream &operator<<(std::ostream &os, const Bound2<T> &n) {
		os << "[" << n.minPoint << " , " << n.maxPoint << "]";
		return os;
	}

};
typedef Bound2<Float> Bound2f;
typedef Bound2<int> Bound2i;

//AABB和point之间的合并
template<typename T>
Bound3<T> Union(const Bound3<T>& b, const Point3<T> p) {
	return Bound3<T>(
			Point3<T>(std::min(b.minPoint.x, p.x), std::min(b.minPoint.y, p.y),
					std::min(b.minPoint.z, p.z)),
			Point3<T>(std::max(b.maxPoint.x, p.x), std::max(b.maxPoint.y, p.y),
					std::max(b.maxPoint.z, p.z)));
}
//AABB和AABB之间的合并
template<typename T>
Bound3<T> Union(const Bound3<T>& b, const Bound3<T>& b2) {
	return Bound3<T>(
			Point3<T>(std::min(b.minPoint.x, b2.minPoint.x),
					std::min(b.minPoint.y, b2.minPoint.y),
					std::min(b.minPoint.z, b2.minPoint.z)),
			Point3<T>(std::max(b.maxPoint.x, b2.maxPoint.x),
					std::max(b.maxPoint.y, b2.maxPoint.y),
					std::max(b.maxPoint.z, b2.maxPoint.z)));
}

//AABB盒之间的交集
template<typename T>
Bound3<T> Intersect(const Bound3<T>& b, const Bound3<T>& b2) {
	return Bound3<T>(
			Point3<T>(std::max(b.minPoint.x, b2.minPoint.x),
					std::max(b.minPoint.y, b2.minPoint.y),
					std::max(b.minPoint.z, b2.minPoint.z)),
			Point3<T>(std::min(b.maxPoint.x, b2.maxPoint.x),
					std::min(b.maxPoint.y, b2.maxPoint.y),
					std::min(b.maxPoint.z, b2.maxPoint.z)));
}

//判断两个AABB盒是否重叠
template<typename T>
bool Overlap(const Bound3<T> &b1, const Bound3<T> &b2) {
	bool x = (b1.maxPoint.x >= b2.minPoint.x)
			&& (b1.minPoint.x <= b2.maxPoint.x);
	bool y = (b1.maxPoint.y >= b2.minPoint.y)
			&& (b1.minPoint.y <= b2.maxPoint.y);
	bool z = (b1.maxPoint.z >= b2.minPoint.z)
			&& (b1.minPoint.z <= b2.maxPoint.z);
	return (x && y && z);
}

//判断一个点是否在AABB中
template<typename T>
bool Inside(const Point3<T>& p, const Bound3<T> &b) {
	bool x = p.x >= b.minPoint.x && p.x <= b.maxPoint.x;
	bool y = p.y >= b.minPoint.y && p.x <= b.maxPoint.y;
	bool z = p.z >= b.minPoint.z && p.z <= b.maxPoint.z;
	return (x && y && z);
}

//判断一个点是否在AABB中,不包括上边界
template<typename T>
bool InsideExclusive(const Point3<T>& p, const Bound3<T> &b) {
	bool x = p.x >= b.minPoint.x && p.x < b.maxPoint.x;
	bool y = p.y >= b.minPoint.y && p.x < b.maxPoint.y;
	bool z = p.z >= b.minPoint.z && p.z < b.maxPoint.z;
	return (x && y && z);
}

//扩充AABB,各个维都扩充delta分量
template<typename T>
Bound3<T> Expand(const Bound3<T>& b, T delta) {
	Bound3<T> result;
	result.minPoint = b.minPoint + Vector3<T>(-delta, -delta, -delta);
	result.maxPoint = b.maxPoint + Vector3<T>(delta, delta, delta);
	return result;
}

//基础射线
class Ray {
public:
	Point3f o;	//射线原点
	Vector3f d;	//射线的方向
	mutable Float tMax; //最大参数值
	Float time; //曝光时间相关
public:
	Ray(const Point3f& oo, const Vector3f& dd, float tmax = Infinity, Float t =
			0.0f) :
			o(oo), d(dd), tMax(tmax), time(t) {
		Assert(!HasNaNs());
	}
	Ray() :
			tMax(Infinity), time(0) {
	}
	Point3f operator()(Float t) const {
		return o + d * t;
	}

	//判断射线是否包含NaN变量
	bool HasNaNs() const {
		return o.HasNaNs() || d.HasNaNs() || IsNaN(time) || IsNaN(tMax);
	}

	//重构ostream方法
	friend std::ostream &operator<<(std::ostream &os, const Ray &r) {
		os << "< o:" << r.o << " ,d:" << r.d << " ,tMax:" << r.tMax << " ,time:"
				<< r.time << ">";
		return os;
	}
};

class RayDifferential: public Ray {
public:
	bool hasDifferential;	//判断是否包含微分信息
	Point3f ox, oy;
	Vector3f dx, dy;
public:
	RayDifferential() :
			Ray() {
		hasDifferential = false;	//默认没有微分信息
	}
	RayDifferential(const Point3f& oo, const Vector3f& dd,
			float tmax = Infinity, Float t = 0.0f) :
			Ray(oo, dd, tmax, t) {
		Assert(!HasNaNs());
		hasDifferential = false;
	}
	RayDifferential(const Ray& r) :
			Ray(r) {
		Assert(!r.HasNaNs());
		hasDifferential = false;
	}

	//判断射线是否包含NaN变量
	bool HasNaNs() const {
		return Ray::HasNaNs() || ox.HasNaNs() || oy.HasNaNs() || dx.HasNaNs()
				|| dy.HasNaNs();
	}

	//缩放微分信息，默认的差分是一个像素的信息(1 dx ==1 pixel)
	void ScaleRayDifferential(Float s) {
		ox = o + (ox - o) * s;
		oy = o + (oy - o) * s;
		dx = d + (dx - d) * s;
		dy = d + (dy - d) * s;
	}

	//重构ostream方法
	friend std::ostream &operator<<(std::ostream &os,
			const RayDifferential &r) {
		os << "< o:" << r.o << " ,d:" << r.d << " ,tMax:" << r.tMax << " ,time:"
				<< r.time << " ,hasDifferential:" << r.hasDifferential
				<< " ,ox:" << r.ox << " ,dx:" << r.dx << " ,oy:" << r.oy
				<< " ,dy:" << r.dy << ">";
		return os;
	}
};

//Dot运算
template<typename T>
inline Float Dot(const Vector3<T>& v1, const Vector3<T>& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
template<typename T>
inline Float Dot(const Vector3<T>& v1, const Normal3<T>& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
template<typename T>
inline Float Dot(const Normal3<T>& v1, const Vector3<T>& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
template<typename T>
inline Float Dot(const Normal3<T>& v1, const Normal3<T>& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
template<typename T>
inline Float Dot(const Vector2<T>& v1, const Vector2<T>& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}
template<typename T>
inline Float AbsDot(const Vector3<T>& v1, const Vector3<T>& v2) {
	return std::abs(Dot(v1, v2));
}
template<typename T>
inline Float AbsDot(const Normal3<T>& v1, const Vector3<T>& v2) {
	return std::abs(Dot(v1, v2));
}
template<typename T>
inline Float AbsDot(const Vector3<T>& v1, const Normal3<T>& v2) {
	return std::abs(Dot(v1, v2));
}
template<typename T>
inline Float AbsDot(const Normal3<T>& v1, const Normal3<T>& v2) {
	return std::abs(Dot(v1, v2));
}
template<typename T>
inline Float AbsDot(const Vector2<T>& v1, const Vector2<T>& v2) {
	return std::abs(Dot(v1, v2));
}
//叉乘
//基于左手坐标系
template<typename T>
inline Vector3<T> Cross(const Vector3<T>& v1, const Vector3<T>& v2) {
	return Vector3<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
}
template<typename T>
inline Vector3<T> Cross(const Normal3<T>& v1, const Vector3<T>& v2) {
	return Vector3<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
}
template<typename T>
inline Vector3<T> Cross(const Normal3<T>& v1, const Normal3<T>& v2) {
	return Vector3<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
}
template<typename T>
inline Vector3<T> Cross(const Vector3<T>& v1, const Normal3<T>& v2) {
	return Vector3<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
}

//标准化
template<typename T>
inline Vector3<T> Normalize(const Vector3<T>& v) {
	return v / v.Magnitude();
}
template<typename T>
inline Vector2<T> Normalize(const Vector2<T>& v) {
	return v / v.Magnitude();
}
template<typename T>
inline Normal3<T> Normalize(const Normal3<T>& n) {
	Assert(!n.HasNaNs());
	return n / n.Length();
}

//标量乘以向量的操作，其实就是换个位置，使用向量乘以标量的方式
template<typename T, typename U>
inline Vector3<T> operator*(U n, const Vector3<T>& v) {
	return v * n;
}

//把第一个参数转换到和第二个参数相同的空间半球中
//4系列
template<typename T>
inline Normal3<T> Faceforward(const Normal3<T> &n, const Vector3<T> &v) {
	return (Dot(n, v) < 0.f) ? -n : n;
}

template<typename T>
inline Normal3<T> Faceforward(const Normal3<T> &n, const Normal3<T> &v) {
	return (Dot(n, v) < 0.f) ? -n : n;
}

template<typename T>
inline Vector3<T> Faceforward(const Vector3<T> &n, const Vector3<T> &v) {
	return (Dot(n, v) < 0.f) ? -n : n;
}

template<typename T>
inline Vector3<T> Faceforward(const Vector3<T> &n, const Normal3<T> &v) {
	return (Dot(n, v) < 0.f) ? -n : n;
}

//距离相关
template<typename T>
inline Float DistanceSquared(const Point3<T>& p1, const Point3<T>& p2) {
	Assert(!p1.HasNaNs() && !p2.HasNaNs());
	return (p1 - p2).MagnitudeSquared();
}
template<typename T>
inline Float Distance(const Point3<T>& p1, const Point3<T>& p2) {
	return sqrt(DistanceSquared(p1, p2));
}
template<typename T>
inline Float DistanceSquared(const Point2<T>& p1, const Point2<T>& p2) {
	Assert(!p1.HasNaNs() && !p2.HasNaNs());
	return (p1 - p2).MagnitudeSquared();
}
template<typename T>
inline Float Distance(const Point2<T>& p1, const Point2<T>& p2) {
	return sqrt(DistanceSquared(p1, p2));
}

//根据一个向量V(x,y,z)生成一个新的坐标系
//1.首先生成一个和V正交的向量VT(-z,0,x),并且标准化
//2.叉乘生成第三个变量VB
//3.为了保证当x,z都为0的时候发生错误，所以这里判断当x>y:VT(-z,0,x)不然VT(0,-z,y)
template<typename T>
inline void CoordinateSystem(const Vector3<T>& V, Vector3<T>* VT,
		Vector3<T>* VB) {
	if (V.x > V.y) {
		Float inv = 1.0f / sqrt(V.x * V.x + V.z * V.z);	//用来标准化的参数
		(*VT) = Vector3<T>(-V.z * inv, 0, V.x * inv);
	} else {
		Float inv = 1.0f / sqrt(V.y * V.y + V.z * V.z);	//用来标准化的参数
		(*VT) = Vector3<T>(0, -V.z * inv, V.y * inv);
	}
	(*VB) = Cross(V, *VT);
}

//球坐标到向量的变换
//theta是和Z轴之间的角度
//phi是从x轴往y轴旋转的角度
//1.costheta就是Z坐标
//2.1-Z=x+y=>x+y=sintheta
//3.不考虑Z轴的情况下,x==cosphi,y==sinphi

inline Vector3f SphericalDirection(Float sintheta, Float costheta, Float phi) {
	return Vector3f(sintheta * cos(phi), sintheta * sin(phi),
			costheta);
}
//任意坐标系下的球面坐标转向量
inline Vector3f SphericalDirection(Float sinTheta, Float cosTheta, Float phi,
		const Vector3f &x, const Vector3f &y, const Vector3f &z) {
	return sinTheta * cos(phi) * x + sinTheta * sin(phi) * y
			+ cosTheta * z;
}

//通过向量返回球面坐标theta
inline Float SphericalTheta(const Vector3f& v) {
	Float z = Clamp(v.z, -1, 1);
	return acos(z);
}

//通过向量返回球面坐标phi
inline Float SphericalPhi(const Vector3f &v) {
	Float p = atan2(v.y, v.x);	//因为phi是0~2PI 所以不能直接考虑cosphi
	return (p < 0) ? (p + 2 * Pi) : p;
}

//返回Vector3<T>的绝对值
template<typename T>
inline Vector3<T> Abs(const Vector3<T>& v) {
	return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

//todo geomtry相关函数的扩充(补充说明:还有一小部分)


#endif //RAYTRACER_GEOMETRY_H
