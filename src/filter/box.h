/*
 * box.h
 *
 *  Created on: 2016年7月5日
 *      Author: Administrator
 */

#ifndef FILTER_BOX_H_
#define FILTER_BOX_H_

#include "../core/seidennki.h"
#include "filter.h"

class BoxFilter:public Filter{
public:
	BoxFilter(float xw,float yw):Filter(xw,yw){}
	virtual float Evaluate(float x,float y) const override{
		return 1.0f;
	}
};



#endif /* FILTER_BOX_H_ */
