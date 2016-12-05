/*
 * box.h
 *
 *  Created on: 2016年7月5日
 *      Author: Administrator
 */

#ifndef FILTER_BOX_H_
#define FILTER_BOX_H_

#include <kumo.h>
#include "filter.h"

class BoxFilter:public Filter{
public:
	BoxFilter(Float xw,Float yw):Filter(xw,yw){}
	virtual Float Evaluate(Float x,Float y) const override{
		return 1.0f;
	}
};



#endif /* FILTER_BOX_H_ */
