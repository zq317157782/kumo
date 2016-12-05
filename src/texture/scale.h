/*
 * scale.h
 *
 *  Created on: 2016年7月25日
 *      Author: Administrator
 */

#ifndef TEXTURE_SCALE_H_
#define TEXTURE_SCALE_H_

#include <kumo.h>
#include "texture.h"


template <typename T1, typename T2>
class ScaleTexture:public Texture<T2>{
private :
	Reference<Texture<T1>> mTex1;
	Reference<Texture<T2>> mTex2;
public:
 ScaleTexture( const Reference<Texture<T1>>& t1, const Reference<Texture<T2>>& t2):mTex1(t1),mTex2(t2){
 }
 virtual T2 Evaluate(const DifferentialGeometry& dg) const override{
	 return mTex1->Evaluate(dg)*mTex2->Evaluate(dg);
 }
 virtual ~ScaleTexture(){};

};





#endif /* TEXTURE_SCALE_H_ */
