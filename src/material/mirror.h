/*
 * mirror.h
 *
 *  Created on: 2016年8月4日
 *      Author: zhuqian
 */

#ifndef MATERIAL_MIRROR_H_
#define MATERIAL_MIRROR_H_
#include "../core/seidennki.h"
#include "material.h"
//镜面材质
class Mirror:public Material{
private:
	Reference<Texture<RGB>> mKr;
public:
	Mirror(const Reference<Texture<RGB>> r){
		mKr=r;
	}
	virtual ~Mirror(){}
	virtual BSDF *GetBSDF(const DifferentialGeometry &dgGeom,
				const DifferentialGeometry &dgShading, MemoryArena &arena) const
						override;
};



#endif /* MATERIAL_MIRROR_H_ */
