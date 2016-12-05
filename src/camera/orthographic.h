/*
 * orthographic.h
 *
 *  Created on: 2016年9月12日
 *      Author: zhuqian
 */

#ifndef SRC_CAMERA_ORTHOGRAPHIC_H_
#define SRC_CAMERA_ORTHOGRAPHIC_H_
//正交相机模型
#include <camera.h>
#include "../core/seidennki.h"
class OrthoCamera: public ProjectiveCamera {
private:
	Vector dxCamera, dyCamera; //相机空间和Raster空间的差分
public:
	OrthoCamera(const Transform& c2w, const Float screenWindow[4], Float lensr,
			Float focald, Film * f);
	virtual Float GenerateRay(const CameraSample &sample, Ray *ray) const
			override;
	virtual Float GenerateRayDifferential(const CameraSample &sample,
			RayDifferential *rd) const;
};

#endif /* SRC_CAMERA_ORTHOGRAPHIC_H_ */
