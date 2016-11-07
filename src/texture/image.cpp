/*
 * image.cpp
 *
 *  Created on: 2016年8月25日
 *      Author: zhuqian
 */
#include <rgb.h>
#include "image.h"
#include "imageio.h"
#include "mipmap.h"
SimpleImageTexture::SimpleImageTexture(TextureMapping2D *m,
		const string& filename,bool doTri,
        float maxAniso,ImageWrap wm, float scale, float gamma) {
	mMapping=m;
	std::vector<unsigned char> image;
	int width;
	int height;
	RGB* data=ReadImage(filename,&width,&height);
	mMipmap = new MIPMap<RGB>(width, height,data, doTri,maxAniso, wm);
}

RGB SimpleImageTexture::Evaluate(const DifferentialGeometry& dg) const{
	 float s, t, dsdx, dtdx, dsdy, dtdy;
	 mMapping->Map(dg, &s, &t, &dsdx, &dtdx, &dsdy, &dtdy);
	 RGB mem = mMipmap->Lookup(s, t, dsdx, dtdx, dsdy, dtdy);
	 return mem;
}

