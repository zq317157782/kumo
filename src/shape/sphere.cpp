//
// Created by 诸谦 on 15/12/28.
//

#include "transform.h"
#include "diffgeom.h"
#include "sphere.h"
#include "montecarlo.h"

bool Sphere::Intersect(const Ray &r, Float *distance, Float *rayEpsilon,
		DifferentialGeometry *dg) const {
	Ray ray;
	(*worldToLocal)(r, &ray);
	// Compute quadratic sphere coefficients
	Float A = ray.d.x * ray.d.x + ray.d.y * ray.d.y + ray.d.z * ray.d.z;
	Float B = 2 * (ray.d.x * ray.o.x + ray.d.y * ray.o.y + ray.d.z * ray.o.z);
	Float C = ray.o.x * ray.o.x + ray.o.y * ray.o.y + ray.o.z * ray.o.z
			- mRad * mRad;
	Float t0, t1;
	if (!Quadratic(A, B, C, &t0, &t1))
		return false;

	// Compute intersection distance along ray
	if (t0 > ray.maxT || t1 < ray.minT)
		return false;
	Float thit = t0;
	if (t0 < ray.minT) {
		thit = t1;
		if (thit > ray.maxT)
			return false;
	}

	//这里开始计算参数化变量

	//计算phi
	Point phit;
	Float phi;
	phit = ray(thit);
	if (phit.x == 0.f && phit.y == 0.f)
		phit.x = 1e-5f * mRad; //排除除零的情况
	phi = atan2f(phit.y, phit.x);
	if (phi < 0.)
		phi += 2.f * Pi; //保证phi在2PI之中

	//判断是否在Z坐标之间的裁剪空间中
	if ((mZMin > -mRad && phit.z < mZMin) || (mZMax < mRad && phit.z > mZMax)
			|| phi > mPhiMax) {
		if (thit == t1)
			return false;
		if (t1 > ray.maxT)
			return false;
		thit = t1;

		phit = ray(thit);
		if (phit.x == 0.f && phit.y == 0.f)
			phit.x = 1e-5f * mRad;
		phi = atan2f(phit.y, phit.x);
		if (phi < 0.)
			phi += 2.f * Pi;
		if ((mZMin > -mRad && phit.z < mZMin)
				|| (mZMax < mRad && phit.z > mZMax) || phi > mPhiMax)
			return false;
	}

	// Find parametric representation of sphere hit
	//寻找参数化的u和v
	Float u = phi / mPhiMax;
	Float theta = acosf(Clamp(phit.z / mRad, -1.f, 1.f));
	Float v = (theta - mThetaMin) / (mThetaMax - mThetaMin);

	// 计算偏导 偏导还不是很熟悉,所以这里照搬了PBRT的公式,详细公式可以查阅PBRT
	Float zradius = sqrtf(phit.x * phit.x + phit.y * phit.y);
	Float invzradius = 1.f / zradius;
	Float cosphi = phit.x * invzradius;
	Float sinphi = phit.y * invzradius;
	Vector3f dpdu(-mPhiMax * phit.y, mPhiMax * phit.x, 0);
	Vector3f dpdv = (mThetaMax - mThetaMin)
			* Vector3f(phit.z * cosphi, phit.z * sinphi, -mRad * sinf(theta));

	//计算法线的偏导
	Vector3f d2Pduu = -mPhiMax * mPhiMax * Vector3f(phit.x, phit.y, 0);
	Vector3f d2Pduv = (mThetaMax - mThetaMin) * phit.z * mPhiMax
			* Vector3f(-sinphi, cosphi, 0.);
	Vector3f d2Pdvv = -(mThetaMax - mThetaMin) * (mThetaMax - mThetaMin)
			* Vector3f(phit.x, phit.y, phit.z);

	Float E = Dot(dpdu, dpdu);
	Float F = Dot(dpdu, dpdv);
	Float G = Dot(dpdv, dpdv);
	Vector3f N = Normalize(Cross(dpdu, dpdv));
	Float e = Dot(N, d2Pduu);
	Float f = Dot(N, d2Pduv);
	Float g = Dot(N, d2Pdvv);

	Float invEGF2 = 1.f / (E * G - F * F);
	Normal dndu = Normal(
			(f * F - e * G) * invEGF2 * dpdu
					+ (e * F - f * E) * invEGF2 * dpdv);
	Normal dndv = Normal(
			(g * F - f * G) * invEGF2 * dpdu
					+ (f * F - g * E) * invEGF2 * dpdv);

	const Transform &o2w = *localToWorld;

	*dg = DifferentialGeometry(o2w(phit), o2w(dpdu), o2w(dpdv), o2w(dndu),
			o2w(dndv), u, v, this);
	*distance = thit;
	*rayEpsilon = 5e-4f * *distance; //交点处的Float误差

	return true;
}

bool Sphere::CanIntersect() const {
	return true;
}

Sphere::Sphere(const Transform *o2w, const Transform *w2o, bool ro, Float rad,
		Float z0, Float z1, Float phiMax) :
		Shape(o2w, w2o, ro), mRad(rad) {

	mZMin = Clamp(std::min(z0, z1), -mRad, mRad);
	mZMax = Clamp(std::max(z0, z1), -mRad, mRad);
	mThetaMin = acosf(Clamp(mZMin / mRad, -1.f, 1.f));
	mThetaMax = acosf(Clamp(mZMax / mRad, -1.f, 1.f));
	mPhiMax = Radians(Clamp(phiMax, 0.0f, 360.0f));
}

//TODO 带参球体面积怎么算
Float Sphere::Area() const {
	return mPhiMax * mRad * (mZMax - mZMin);
}

Point Sphere::Sample(Float u1, Float u2, Normal *Ns) const {
	Point p = Point(0, 0, 0) + mRad * UniformSampleSphere(u1, u2);
	*Ns = Normalize((*localToWorld)(Normal(p.x, p.y, p.z))); //TODO 这样转换法线正确吗？
	if (ReverseOrientation)
		*Ns *= -1.f;
	return (*localToWorld)(p);
}

bool Sphere::IntersectP(const Ray& r) const {
	Ray ray;
	(*worldToLocal)(r, &ray);
	// Compute quadratic sphere coefficients
	Float A = ray.d.x * ray.d.x + ray.d.y * ray.d.y + ray.d.z * ray.d.z;
	Float B = 2 * (ray.d.x * ray.o.x + ray.d.y * ray.o.y + ray.d.z * ray.o.z);
	Float C = ray.o.x * ray.o.x + ray.o.y * ray.o.y + ray.o.z * ray.o.z
			- mRad * mRad;
	Float t0, t1;
	if (!Quadratic(A, B, C, &t0, &t1))
		return false;

	// Compute intersection distance along ray
	if (t0 > ray.maxT || t1 < ray.minT)
		return false;
	Float thit = t0;
	if (t0 < ray.minT) {
		thit = t1;
		if (thit > ray.maxT)
			return false;
	}

	//这里开始计算参数化变量

	//计算phi
	Point phit;
	Float phi;
	phit = ray(thit);
	if (phit.x == 0.f && phit.y == 0.f)
		phit.x = 1e-5f * mRad; //排除除零的情况
	phi = atan2f(phit.y, phit.x);
	if (phi < 0.)
		phi += 2.f * Pi; //保证phi在2PI之中

	//判断是否在Z坐标之间的裁剪空间中
	if ((mZMin > -mRad && phit.z < mZMin) || (mZMax < mRad && phit.z > mZMax)
			|| phi > mPhiMax) {
		if (thit == t1)
			return false;
		if (t1 > ray.maxT)
			return false;
		thit = t1;

		phit = ray(thit);
		if (phit.x == 0.f && phit.y == 0.f)
			phit.x = 1e-5f * mRad;
		phi = atan2f(phit.y, phit.x);
		if (phi < 0.0)
			phi += 2.f * Pi;
		if ((mZMin > -mRad && phit.z < mZMin)
				|| (mZMax < mRad && phit.z > mZMax) || phi > mPhiMax)
			return false;
	}
	return true;
}
