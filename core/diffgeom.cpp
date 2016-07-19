//
// Created by 诸谦 on 16/6/18.
//

#include "diffgeom.h"
#include "shape.h"

DifferentialGeometry::DifferentialGeometry(const Point &P, const Vector &DPDU,
		const Vector &DPDV, const Normal &DNDU, const Normal &DNDV, float uu,
		float vv, const Shape *sh) :
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

void DifferentialGeometry::ComputeDifferentials(
		const RayDifferential &r) const {
	if (r.hasDifferentials) { //判断射线是否有微分射线存在否
		//计算主射线交点处的 切平面
		//然后计算  辅助射线与切平面的交点，辅助交点
		//辅助交点和原交点的差可以近似成 偏导
		float d = -Dot(nn, Vector(p.x, p.y, p.z));
		Vector rxv(r.rxOrigin.x, r.rxOrigin.y, r.rxOrigin.z);
		float tx = -(Dot(nn, rxv) + d) / Dot(nn, r.rxDirection);
		if (isnan(tx))
			goto fail;
		Point px = r.rxOrigin + tx * r.rxDirection;
		Vector ryv(r.ryOrigin.x, r.ryOrigin.y, r.ryOrigin.z);
		float ty = -(Dot(nn, ryv) + d) / Dot(nn, r.ryDirection);
		if (isnan(ty))
			goto fail;
		Point py = r.ryOrigin + ty * r.ryDirection;
		dpdx = px - p; //x方向上的偏导
		dpdy = py - p; //y方向上的偏导
		//todo 这个函数  还没有全部完成
	} else {
fail:
		dpdx = Vector(0, 0, 0);
		dpdy = Vector(0, 0, 0);
		dudx = 0;
		dudy = 0;
		dvdx = 0;
		dvdy = 0;
	}
}
