/*
 * perspective.h
 *
 *  Created on: 2016年9月12日
 *      Author: zhuqian
 */

#ifndef SRC_CAMERA_PERSPECTIVE_H_
#define SRC_CAMERA_PERSPECTIVE_H_
#include <camera.h>
#include "global.h"
class PerspectiveCamera: public ProjectiveCamera {
private:
	Vector dxCamera, dyCamera; //相机空间和Raster空间的差分
public:
	PerspectiveCamera(const Transform& c2w, const float screenWindow[4], float lensr,
			float focald, float fov,Film * f);
	virtual float GenerateRay(const CameraSample &sample, Ray *ray) const
			override;
	virtual float GenerateRayDifferential(const CameraSample &sample,
			RayDifferential *rd) const;
};

#endif /* SRC_CAMERA_PERSPECTIVE_H_ */
