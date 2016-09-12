//
// Created by 诸谦 on 15/12/28.
//

#include "Camera.h"
#include "film.h"
#include "geometry.h"
#include "sampler.h"

Camera::Camera(const Transform& c2w, Film * f) :
		film(f), cameraToWorld(c2w) {

}

float Camera::GenerateRayDifferential(const CameraSample &sample,
		RayDifferential *rd) const {
	float wt = GenerateRay(sample, rd);
	// Find ray after shifting one pixel in the $x$ direction
	CameraSample sshift = sample;
	++(sshift.imageX);
	Ray rx;
	float wtx = GenerateRay(sshift, &rx);
	rd->rxOrigin = rx.o;
	rd->rxDirection = rx.d;

	// Find ray after shifting one pixel in the $y$ direction
	--(sshift.imageX);
	++(sshift.imageY);
	Ray ry;
	float wty = GenerateRay(sshift, &ry);
	rd->ryOrigin = ry.o;
	rd->ryDirection = ry.d;
	if (wtx == 0.0f || wty == 0.0f)
		return 0.0f;
	rd->hasDifferentials = true;
	return wt;
}

ProjectiveCamera::ProjectiveCamera(const Transform& c2w, const Transform& proj,
		const float screenWindow[4], float lensr, float focald, Film * f) :
		Camera(c2w, f) {
	CameraToScreen = proj; //投影矩阵
	lensRadius = lensr;
	focalDistance = focald;
	//从底往上看1.把screen的原点挪到00位置,然后你懂得
	ScreenToRaster = Scale(float(film->xResolution), float(film->yResolution),
			1.f)
			* Scale(1.f / (screenWindow[1] - screenWindow[0]),
					1.f / (screenWindow[2] - screenWindow[3]), 1.f)
			* Translate(Vector(-screenWindow[0], -screenWindow[3], 0.f));
	 RasterToScreen = Inverse(ScreenToRaster);
	 RasterToCamera=Inverse(CameraToScreen)*RasterToScreen;

}
