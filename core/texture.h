/*
 * texture.h
 *
 *  Created on: 2016年7月24日
 *      Author: zhuqian
 */

#ifndef CORE_TEXTURE_H_
#define CORE_TEXTURE_H_
#include "global.h"

/*纹理基类*/
template <typename T>
class Texture:public ReferenceCounted{
public:
virtual T Evaluate(const DifferentialGeometry&) const =0;
virtual ~Texture(){};
};


template <typename T>
class ConstantTexture:public Texture{
private :
 T	mValue;
public:
 virtual T Evaluate(const DifferentialGeometry&) const override{
	 return mValue;
 }
 virtual ~ConstantTexture(){};

};

#endif /* CORE_TEXTURE_H_ */
