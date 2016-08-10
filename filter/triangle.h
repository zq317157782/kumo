/*
 * triangle.h
 *
 *  Created on: 2016年8月10日
 *      Author: zhuqian
 */

#ifndef FILTER_TRIANGLE_H_
#define FILTER_TRIANGLE_H_


#include "global.h"
#include "filter.h"

class TriangleFilter:public Filter{
public:
	TriangleFilter(float xw,float yw):Filter(xw,yw){}
	virtual float Evaluate(float x,float y) const override{
		float zero=0.0f;
		return max(0.0f,(float)(xWidth-fabs(x)))*max(0.0f,(float)(yWidth-fabs(y)));
	}
};



#endif /* FILTER_TRIANGLE_H_ */
