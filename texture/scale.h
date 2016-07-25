/*
 * scale.h
 *
 *  Created on: 2016年7月25日
 *      Author: Administrator
 */

#ifndef TEXTURE_SCALE_H_
#define TEXTURE_SCALE_H_

#include "global.h"
#include "texture.h"


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
};

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
