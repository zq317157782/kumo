/*
 * orthographic.cpp
 *
 *  Created on: 2016年9月12日
 *      Author: zhuqian
 */
#include "orthographic.h"
#include "geometry.h"
#include "sampler.h"
#include "montecarlo.h"

OrthoCamera::OrthoCamera(const Transform& c2w, const float screenWindow[4],
		float lensr, float focald, Film * f) :
		ProjectiveCamera(c2w, Orthographic(0.0f, 1.0f), screenWindow, lensr,
				focald, f) {
	dxCamera = RasterToCamera(Vector(1, 0, 0));
	dyCamera = RasterToCamera(Vector(0, 1, 0));
}

float OrthoCamera::GenerateRay(const CameraSample &sample, Ray *ray) const {
	Point pRas(sample.imageX, sample.imageY, 0);
	Point pCam;
	RasterToCamera(pRas, &pCam);
	*ray = Ray(pCam, Vector(0, 0, 1), 0.0f, INFINITY);
	if (lensRadius > 0.0f) {
		float lensU, lensV;
		ConcentricSampleDisk(sample.lensU, sample.lensV, &lensU, &lensV); //采样Lens
		lensU *= lensRadius;
		lensV *= lensRadius;

		//计算焦距平面交点
		float ft = focalDistance / ray->d.z;
		Point pFocus = (*ray)(ft);

		ray->o = Point(lensU, lensV, 0.0f);
		ray->d = Normalize(pFocus - ray->o);
	}
	cameraToWorld(*ray, ray);
	return 1.0f;
}

//微分相机射线
float OrthoCamera::GenerateRayDifferential(const CameraSample &sample,
		RayDifferential *ray) const {
	Point pRas(sample.imageX, sample.imageY, 0);
	Point pCam;
	RasterToCamera(pRas, &pCam);
	*ray = RayDifferential(pCam, Vector(0, 0, 1), 0.0f, INFINITY);
	if (lensRadius > 0.0f) {
		float lensU, lensV;
		ConcentricSampleDisk(sample.lensU, sample.lensV, &lensU, &lensV); //采样Lens
		lensU *= lensRadius;
		lensV *= lensRadius;

		//计算焦距平面交点
		float ft = focalDistance / ray->d.z;
		Point pFocus = (*ray)(ft);

		ray->o = Point(lensU, lensV, 0.0f);
		ray->d = Normalize(pFocus - ray->o);
	}

	if (lensRadius > 0.0f) {
		//有景深版本
		float lensU, lensV;
		ConcentricSampleDisk(sample.lensU, sample.lensV, &lensU, &lensV); //采样Lens
		lensU *= lensRadius;
		lensV *= lensRadius;

		float ft = focalDistance / ray->d.z;

		Point pFocus = pCam + dxCamera + (ft * Vector(0, 0, 1));
		ray->rxOrigin = Point(lensU, lensV, 0.0f);
		ray->rxDirection = Normalize(pFocus - ray->rxOrigin);

		pFocus = pCam + dyCamera + (ft * Vector(0, 0, 1));
		ray->ryOrigin = Point(lensU, lensV, 0.0f);
		ray->ryDirection = Normalize(pFocus - ray->rxOrigin);
	} else {
		//没有景深版本
		ray->rxOrigin = ray->o + dxCamera;
		ray->ryOrigin = ray->o + dyCamera;
		ray->rxDirection = ray->ryDirection = ray->d;
	}
	ray->hasDifferentials = true;
	cameraToWorld(*ray, ray);
	return 1.0f;
}

