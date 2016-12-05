/*
 * texture.h
 *
 *  Created on: 2016年7月24日
 *      Author: zhuqian
 */

#ifndef CORE_TEXTURE_H_
#define CORE_TEXTURE_H_
#include "diffgeom.h"
#include "memory.h"
#include "seidennki.h"


//计算坐标纹理映射的结构
class TextureMapping2D{
public:
	virtual void Map(const DifferentialGeometry& dg,float* s,float *t,float *dsdx,float *dtdx,float *dsdy,float *dtdy) const=0;
	~TextureMapping2D(){};
};


//根据参数uv来计算纹理坐标映射
class UVMapping2D:public TextureMapping2D{
private:
	float mScaleU,mScaleV;
	float mDeltaU,mDeltaV;
public:
	UVMapping2D(float su=1, float sv=1, float du=0, float dv=0):mScaleU(su),mScaleV(sv),mDeltaU(du),mDeltaV(dv){

	}

	virtual void Map(const DifferentialGeometry& dg,float* s,float *t,float *dsdx,float *dtdx,float *dsdy,float *dtdy) const override{
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

float Lanczos(float x, float tau=2);

#endif /* CORE_TEXTURE_H_ */
