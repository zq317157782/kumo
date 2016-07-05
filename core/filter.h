/*
 * filter.h
 *
 *  Created on: 2016年7月5日
 *      Author: Administrator
 */

#ifndef CORE_FILTER_H_
#define CORE_FILTER_H_

//过滤器
class Filter {
public:
	virtual ~Filter() {
	}
	;
	const float xWidth, yWidth;
	const float invXWidth, invYWidth;

	Filter(float xw,float yw):xWidth(xw),yWidth(yw),invXWidth(1.0f/xw),invYWidth(1.0f/yw){}

	virtual float Evaluate(float x,float y) const =0;//返回相应位置的过滤器权重
};

#endif /* CORE_FILTER_H_ */
