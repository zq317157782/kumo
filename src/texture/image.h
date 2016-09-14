/*
 * image.h
 *
 *  Created on: 2016年8月24日
 *      Author: zhuqian
 */

#ifndef SRC_TEXTURE_IMAGE_H_
#define SRC_TEXTURE_IMAGE_H_
//IMAGE纹理
#include "texture.h"
#include "mipmap.h"
    class PNGImageTexture : public Texture<RGB> {
private:
	MIPMap<RGB> *mMipmap;
	TextureMapping2D *mMapping;
public:
	PNGImageTexture(TextureMapping2D *m,
			const char*,bool doTri,
            float maxAniso,ImageWrap wm, float scale, float gamma);
	virtual RGB Evaluate(const DifferentialGeometry&) const override;
};



#endif /* SRC_TEXTURE_IMAGE_H_ */
