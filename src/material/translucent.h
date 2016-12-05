/*
 *  translucent.h
 *
 *  Created on: 2016年8月4日
 *      Author: zhuqian
 */

#ifndef MATERIAL_TRANSLUCENT_H_
#define MATERIAL_TRANSLUCENT_H_
#include "../core/seidennki.h"
#include "material.h"

class Translucent: public Material {
private:
	Reference<Texture<RGB>> mKd, mKs; //漫反射系数和镜面反射系数
	Reference<Texture<float>> mRoughness; //粗糙度
	Reference<Texture<RGB>> mReflect; //反射系数
	Reference<Texture<RGB>> mTransmit; //折射系数
public:
	Translucent(Reference<Texture<RGB> > kd,
			Reference<Texture<RGB> > ks, Reference<Texture<float>> rough,
			Reference<Texture<RGB> > refl,
			Reference<Texture<RGB> > trans){
		mKd=kd;
		mKs=ks;
		mRoughness=rough;
		mReflect=refl;
		mTransmit=trans;
	}

	virtual BSDF *GetBSDF(const DifferentialGeometry &dgGeom,
					const DifferentialGeometry &dgShading, MemoryArena &arena) const
							override;

	virtual ~Translucent(){}
};

#endif /* MATERIAL_TRANSLUCENT_H_ */
