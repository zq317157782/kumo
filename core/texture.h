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


#endif /* CORE_TEXTURE_H_ */
