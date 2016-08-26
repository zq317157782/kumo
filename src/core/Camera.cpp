//
// Created by 诸谦 on 15/12/28.
//

#include "Camera.h"
#include "film.h"
#include "geometry.h"
#include "sampler.h"

Camera::Camera(Film * f, Transform *c2w) :
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
