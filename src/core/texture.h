/*
 * texture.h
 *
 *  Created on: 2016年7月24日
 *      Author: zhuqian
 */

#ifndef CORE_TEXTURE_H_
#define CORE_TEXTURE_H_
#include <kumo.h>
#include "diffgeom.h"
#include "memory.h"


//计算坐标纹理映射的结构
class TextureMapping2D{
public:
	virtual void Map(const DifferentialGeometry& dg,Float* s,Float *t,Float *dsdx,Float *dtdx,Float *dsdy,Float *dtdy) const=0;
	~TextureMapping2D(){};
};


//根据参数uv来计算纹理坐标映射
class UVMapping2D:public TextureMapping2D{
private:
	Float mScaleU,mScaleV;
	Float mDeltaU,mDeltaV;
public:
	UVMapping2D(Float su=1, Float sv=1, Float du=0, Float dv=0):mScaleU(su),mScaleV(sv),mDeltaU(du),mDeltaV(dv){

	}

	virtual void Map(const DifferentialGeometry& dg,Float* s,Float *t,Float *dsdx,Float *dtdx,Float *dsdy,Float *dtdy) const override{
		//根据uv坐标计算st坐标
		*s=mScaleU*dg.u+mDeltaU;
		*t=mScaleV*dg.v+mDeltaV;

		//计算s和t的偏导
		*dsdx=mScaleU*dg.dudx;
		*dsdy=mScaleU*dg.dudy;
		*dtdx=mScaleV*dg.dvdx;
		*dtdy=mScaleV*dg.dvdy;

	}

	virtual ~UVMapping2D(){}
};



/*纹理基类*/
template <typename T>
class Texture:public ReferenceCounted{
public:
virtual T Evaluate(const DifferentialGeometry&) const =0;
virtual ~Texture(){};
};

Float Lanczos(Float x, Float tau=2);

#endif /* CORE_TEXTURE_H_ */
