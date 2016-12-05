/*
 * grass.h
 *
 *  Created on: 2016年8月23日
 *      Author: zhuqian
 */

#ifndef SRC_MATERIAL_GRASS_H_
#define SRC_MATERIAL_GRASS_H_
#include "../core/seidennki.h"
#include "material.h"
//玻璃材质
class Grass:public Material{
private:
	 Reference<Texture<RGB> > mKr, mKt;
	 Reference<Texture<float> > mIndex;
public:
	Grass(Reference<Texture<RGB> > r, Reference<Texture<RGB> > t,
            Reference<Texture<float> > i){
		mKr=r;
		mKt=t;
		mIndex=i;
	}
	virtual ~Grass(){}
	virtual BSDF *GetBSDF(const DifferentialGeometry &dgGeom,
				const DifferentialGeometry &dgShading, MemoryArena &arena) const
						override;
};


#endif /* SRC_MATERIAL_GRASS_H_ */
