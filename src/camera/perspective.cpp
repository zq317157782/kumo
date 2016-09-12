/*
 * perspective.cpp
 *
 *  Created on: 2016年9月12日
 *      Author: zhuqian
 */
#include "perspective.h"
#include "sampler.h"
#include "montecarlo.h"
PerspectiveCamera::PerspectiveCamera(const Transform& c2w,
		const float screenWindow[4], float lensr, float focald, float fov,
		Film * f) :
		ProjectiveCamera(c2w, Perspective(fov, 1e-2f, 1000.f), screenWindow,
				lensr, focald, f) {
	dxCamera = RasterToCamera(Point(1, 0, 0)) - RasterToCamera(Point(0, 0, 0));
	dyCamera = RasterToCamera(Point(0, 1, 0)) - RasterToCamera(Point(0, 0, 0));
}

float PerspectiveCamera::GenerateRay(const CameraSample &sample,
		Ray *ray) const {
	Point pRas(sample.imageX, sample.imageY, 0);
	Point pCam;
	RasterToCamera(pRas, &pCam);
	*ray = Ray(Point(0, 0, 0), Normalize(Vector(pCam)), 0.f, INFINITY);
	if (lensRadius > 0.0) {
		float lensU, lensV;
		ConcentricSampleDisk(sample.lensU, sample.lensV, &lensU, &lensV);
		lensU *= lensRadius;
		lensV *= lensRadius;

		float ft = focalDistance / ray->d.z;
		Point pFocus = (*ray)(ft);

		ray->o = Point(lensU, lensV, 0.0f);
		ray->d = Normalize(pFocus - ray->o);
	}
	cameraToWorld(*ray, ray);
	return 1.0f;
}

float PerspectiveCamera::GenerateRayDifferential(const CameraSample &sample,
		RayDifferential *ray) const {
	Point pRas(sample.imageX, sample.imageY, 0);
	Point pCamera;
	RasterToCamera(pRas, &pCamera);
	Vector dir = Normalize(Vector(pCamera.x, pCamera.y, pCamera.z));
	*ray = RayDifferential(Point(0, 0, 0), dir, 0.f, INFINITY);
	if (lensRadius > 0.0) {
		float lensU, lensV;
		ConcentricSampleDisk(sample.lensU, sample.lensV, &lensU, &lensV);
		lensU *= lensRadius;
		lensV *= lensRadius;

		float ft = focalDistance / ray->d.z;
		Point Pfocus = (*ray)(ft);

		ray->o = Point(lensU, lensV, 0.0f);
		ray->d = Normalize(Pfocus - ray->o);
	}

	if (lensRadius > 0.0f) {
		float lensU, lensV;
		ConcentricSampleDisk(sample.lensU, sample.lensV, &lensU, &lensV);
		lensU *= lensRadius;
		lensV *= lensRadius;

		Vector dx = Normalize(Vector(pCamera + dxCamera));
		float ft = focalDistance / dx.z;//参数焦点
		Point pFocus = Point(0, 0, 0) + (ft * dx);//焦点
		ray->rxOrigin = Point(lensU, lensV, 0.0f);
		ray->rxDirection = Normalize(pFocus - ray->rxOrigin);

		Vector dy = Normalize(Vector(pCamera + dyCamera));
		ft = focalDistance / dy.z;//参数焦点
		pFocus = Point(0, 0, 0) + (ft * dy);//焦点
		ray->ryOrigin = Point(lensU, lensV, 0.0f);
		ray->ryDirection = Normalize(pFocus - ray->ryOrigin);
	} else {
		ray->rxOrigin = ray->ryOrigin = ray->o;
		ray->rxDirection = Normalize(Vector(pCamera) + dxCamera);
		ray->ryDirection = Normalize(Vector(pCamera) + dyCamera);
	}
	cameraToWorld(*ray, ray);
	ray->hasDifferentials = true;
	return 1.0f;
}
