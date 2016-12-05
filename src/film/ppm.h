/*
 * ppm.h
 *
 *  Created on: 2016年7月5日
 *      Author: Administrator
 */

#ifndef FILM_PPM_H_
#define FILM_PPM_H_

#include "kumo.h"
#include "film.h"

class PPMFilm:public Film{
private:
	Filter* mFilter;
	const char* mFileName;
	struct Pixel{
		Float r,g,b;
		Float weightSum;
		Pixel():r(0),g(0),b(0),weightSum(0){}
	};
	Pixel * mPixels;
public:
	PPMFilm(int xres,int yres,Filter* f,const char* file);
	virtual void AddSample(const CameraSample& sample,const RGB& L) override;
	virtual void WriteImage(Float splatScale = 1.f) override;
};



#endif /* FILM_PPM_H_ */
