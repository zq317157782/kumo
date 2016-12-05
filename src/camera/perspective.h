/*
 * perspective.h
 *
 *  Created on: 2016年9月12日
 *      Author: zhuqian
 */

#ifndef SRC_CAMERA_PERSPECTIVE_H_
#define SRC_CAMERA_PERSPECTIVE_H_
#include <camera.h>
#include "kumo.h"
class PerspectiveCamera: public ProjectiveCamera {
private:
	Vector3f dxCamera, dyCamera; //相机空间和Raster空间的差分
public:
	PerspectiveCamera(const Transform& c2w, const Float screenWindow[4], Float lensr,
			Float focald, Float fov,Film * f);
	virtual Float GenerateRay(const CameraSample &sample, Ray *ray) const
			override;
	virtual Float GenerateRayDifferential(const CameraSample &sample,
			RayDifferential *rd) const;
};

#endif /* SRC_CAMERA_PERSPECTIVE_H_ */
