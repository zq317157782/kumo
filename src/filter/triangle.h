/*
 * triangle.h
 *
 *  Created on: 2016年8月10日
 *      Author: zhuqian
 */

#ifndef FILTER_TRIANGLE_H_
#define FILTER_TRIANGLE_H_


#include "kumo.h"
#include "filter.h"

class TriangleFilter:public Filter{
public:
	TriangleFilter(Float xw,Float yw):Filter(xw,yw){}
	virtual Float Evaluate(Float x,Float y) const override{
		Float zero=0.0f;
		return max(0.0f,(Float)(xWidth-fabs(x)))*max(0.0f,(Float)(yWidth-fabs(y)));
	}
};



#endif /* FILTER_TRIANGLE_H_ */
