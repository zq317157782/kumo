/*
 * constant.h
 *
 *  Created on: 2016年7月24日
 *      Author: zhuqian
 */

#ifndef TEXTURE_CONSTANT_H_
#define TEXTURE_CONSTANT_H_

#include "../core/seidennki.h"
#include "texture.h"


template <typename T>
class ConstantTexture:public Texture<T>{
private :
 T	mValue;
public:
 ConstantTexture(const T& _v){
	 mValue=_v;
 }
 virtual T Evaluate(const DifferentialGeometry&) const override{
	 return mValue;
 }
 virtual ~ConstantTexture(){};

};



#endif /* TEXTURE_CONSTANT_H_ */
