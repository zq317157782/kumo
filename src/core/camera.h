//
// Created by 诸谦 on 15/12/28.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include <kumo.h>
#include "transform.h"
class Camera {
public:
	Film * film;
	Transform cameraToWorld;
public:
	Camera(const Transform& c2w, Film * f);
	virtual Float GenerateRay(const CameraSample &sample, Ray *ray) const = 0;
	virtual Float GenerateRayDifferential(const CameraSample &sample,
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
	Float lensRadius, focalDistance;//镜片半径,焦距
public:
	ProjectiveCamera(const Transform& c2w, const Transform& proj,
			const Float screenWindow[4], Float lensr, Float focald, Film * f);
};

#endif //RAYTRACER_CAMERA_H
