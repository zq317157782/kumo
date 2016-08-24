/*
 * checkerboard.h
 *
 *  Created on: 2016年7月25日
 *      Author: Administrator
 */

#ifndef TEXTURE_CHECKERBOARD_H_
#define TEXTURE_CHECKERBOARD_H_

#include "global.h"
#include "texture.h"
//黑白纹理
template<typename T>
class Checkerboard2DTexture: public Texture<T> {
private:
	TextureMapping2D* mMapping;
	Reference<Texture<T> > tex1;
	Reference<Texture<T> > tex2;
public:
	Checkerboard2DTexture(TextureMapping2D *m, Reference<Texture<T> > t1,
			Reference<Texture<T> > t2) :
			mMapping(m), tex1(t1), tex2(t2) {

	}

	virtual T Evaluate(const DifferentialGeometry& dg) const override {
		//计算st坐标
		float s, t, dsdx, dtdx, dsdy, dtdy;
		mMapping->Map(dg, &s, &t, &dsdx, &dtdx, &dsdy, &dtdy);
		if ((Floor2Int(s) + Floor2Int(t)) % 2 == 0)
			return tex1->Evaluate(dg);
		return tex2->Evaluate(dg);
	}
};

#endif /* TEXTURE_CHECKERBOARD_H_ */
