//
// Created by 诸谦 on 16/6/18.
//

#include <shape.h>
#include "diffgeom.h"
#include "transform.h"

DifferentialGeometry::DifferentialGeometry(const Point &P, const Vector3f &DPDU,
		const Vector3f &DPDV, const Normal &DNDU, const Normal &DNDV, Float uu,
		Float vv, const Shape *sh) :
		p(P), dpdu(DPDU), dpdv(DPDV), dndu(DNDU), dndv(DNDV) {
	// Initialize _DifferentialGeometry_ from parameters
	nn = Normal(Normalize(Cross(dpdu, dpdv)));
	u = uu;
	v = vv;
	shape = sh;
	dudx = dvdx = dudy = dvdy = 0;

	// Adjust normal based on orientation and handedness
	if (shape && (shape->ReverseOrientation ^ shape->TransformSwapsHandedness))
		nn *= -1.f;
}

//本函数直接复制自PBRT
void DifferentialGeometry::ComputeDifferentials(
		const RayDifferential &r) const {
	if (r.hasDifferentials) { //判断射线是否有微分射线存在否
		//计算主射线交点处的 切平面
		//然后计算  辅助射线与切平面的交点，辅助交点
		//辅助交点和原交点的差可以近似成 偏导
		Float d = -Dot(nn, Vector3f(p.x, p.y, p.z));
		Vector3f rxv(r.rxOrigin.x, r.rxOrigin.y, r.rxOrigin.z);
		Float tx = -(Dot(nn, rxv) + d) / Dot(nn, r.rxDirection);
		if (isnan(tx))
			goto fail;
		Point px = r.rxOrigin + tx * r.rxDirection;
		Vector3f ryv(r.ryOrigin.x, r.ryOrigin.y, r.ryOrigin.z);
		Float ty = -(Dot(nn, ryv) + d) / Dot(nn, r.ryDirection);
		if (isnan(ty))
			goto fail;
		Point py = r.ryOrigin + ty * r.ryDirection;
		dpdx = px - p; //x方向上的偏导
		dpdy = py - p; //y方向上的偏导


		//开始求解dudx,dudy,dvdx,dvdy  公式 P.508
		Float A[2][2], Bx[2], By[2];
		int axes[2];

		//这里的代码是用来确定哪一行是退化的，退化意味着线性相关，参与运算的话 行列式为0，不可逆，无法求解
		if (fabsf(nn.x) > fabsf(nn.y) && fabsf(nn.x) > fabsf(nn.z)) {
			axes[0] = 1;
			axes[1] = 2;
		} else if (fabsf(nn.y) > fabsf(nn.z)) {
			axes[0] = 0;
			axes[1] = 2;
		} else {
			axes[0] = 0;
			axes[1] = 1;
		}

		A[0][0] = dpdu[axes[0]];
		A[0][1] = dpdv[axes[0]];
		A[1][0] = dpdu[axes[1]];
		A[1][1] = dpdv[axes[1]];
		Bx[0] = px[axes[0]] - p[axes[0]];
		Bx[1] = px[axes[1]] - p[axes[1]];
		By[0] = py[axes[0]] - p[axes[0]];
		By[1] = py[axes[1]] - p[axes[1]];
		if (!SolveLinearSystem2x2(A, Bx, &dudx, &dvdx)) {
			dudx = 0.;
			dvdx = 0.;
		}
		if (!SolveLinearSystem2x2(A, By, &dudy, &dvdy)) {
			dudy = 0.;
			dvdy = 0.;
		}
	} else {
		fail: dpdx = Vector3f(0, 0, 0);
		dpdy = Vector3f(0, 0, 0);
		dudx = 0;
		dudy = 0;
		dvdx = 0;
		dvdy = 0;
	}
}
