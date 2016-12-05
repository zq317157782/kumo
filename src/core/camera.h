//
// Created by 诸谦 on 15/12/28.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "seidennki.h"
#include "transform.h"
class Camera {
public:
	Film * film;
	Transform cameraToWorld;
public:
	Camera(const Transform& c2w, Film * f);
	virtual float GenerateRay(const CameraSample &sample, Ray *ray) const = 0;
	virtual float GenerateRayDifferential(const CameraSample &sample,
			RayDifferential *rd) const;

	virtual ~Camera() {
	}
};

/*投影相机*/
class ProjectiveCamera: public Camera {
protected:
	/*4个变换矩阵*/
	Transform CameraToScreen, RasterToCamera;
	Transform ScreenToRaster, RasterToScreen;
	float lensRadius, focalDistance;//镜片半径,焦距
public:
	ProjectiveCamera(const Transform& c2w, const Transform& proj,
			const float screenWindow[4], float lensr, float focald, Film * f);
};

#endif //RAYTRACER_CAMERA_H
