/*
 * png.h
 *
 *  Created on: 2016年8月11日
 *      Author: zhuqian
 */

#ifndef FILM_PNG_H_
#define FILM_PNG_H_
#include "global.h"
#include "film.h"

class PNGFilm:public Film{
private:
	Filter* mFilter;
	const char* mFileName;
	struct Pixel{
		float r,g,b;
		float sr, sg, sb;
		float weightSum;
		Pixel():r(0),g(0),b(0),sr(0), sg(0), sb(0),weightSum(0){}
	};
	Pixel * mPixels;
public:
	PNGFilm(int xres,int yres,Filter* f,const char* file);
	virtual void AddSample(const CameraSample& sample,const RGB& L) override;
	virtual void WriteImage(float splatScale = 1.f) override;
	virtual void Splat(const CameraSample &sample, const RGB &L) override;
};




#endif /* FILM_PNG_H_ */
