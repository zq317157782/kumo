/*
 * film.h
 *
 *  Created on: 2016年7月5日
 *      Author: Administrator
 */

#ifndef CORE_FILM_H_
#define CORE_FILM_H_
#include "global.h"

class Film{
public:
	const int xResolution,yResolution;
	Film(int xres,int yres):xResolution(xres),yResolution(yres){}
	virtual ~Film();
	virtual void AddSample(const CameraSample& sample,const RGB& L)=0;
	virtual void WriteImage(float splatScale = 1.f);
};




#endif /* CORE_FILM_H_ */
