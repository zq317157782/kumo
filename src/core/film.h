/*
 * film.h
 *
 *  Created on: 2016年7月5日
 *      Author: Administrator
 */

#ifndef CORE_FILM_H_
#define CORE_FILM_H_
#include "global.h"

class Film {
public:
	const int xResolution, yResolution;
	Film(int xres, int yres) :
			xResolution(xres), yResolution(yres) {
	}
	virtual ~Film() {
	}

	virtual void AddSample(const CameraSample& sample, const RGB& L)=0;
	virtual void WriteImage(float splatScale = 1.f) {
	}
	;
	virtual void GetSampleExtent(int *xstart, int *xend, int *ystart,
			int *yend) const {
		*xstart = 0;
		*xend = xResolution - 1;
		*ystart = 0;
		*yend = yResolution - 1;

	}

	virtual void GetPixelExtent(int *xstart, int *xend, int *ystart,
			int *yend) const {
		*xstart = 0;
		*xend = xResolution - 1;
		*ystart = 0;
		*yend = yResolution - 1;

	}

	virtual void Splat(const CameraSample &sample, const RGB &L){};
};

#endif /* CORE_FILM_H_ */
