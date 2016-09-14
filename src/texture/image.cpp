/*
 * image.cpp
 *
 *  Created on: 2016年8月25日
 *      Author: zhuqian
 */
#include "image.h"
#include "../thrid/lodepng/lodepng.h"
#include "RGB.h"
#include "mipmap.h"
PNGImageTexture::PNGImageTexture(TextureMapping2D *m,
		const char* filename,bool doTri,
        float maxAniso,ImageWrap wm, float scale, float gamma) {
	mMapping=m;
	std::vector<unsigned char> image;
	unsigned int width;
	unsigned int height;
	unsigned error = lodepng::decode(image, width, height, filename);
	if(error){
		std::cerr << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		exit(-1);
	}
	RGB* data=new RGB[width*height];
	for(int i=0;i<width*height;++i){
		data[i].r=((float)image[i*4])/255;
		data[i].g=((float)image[i*4+1])/255;
		data[i].b=((float)image[i*4+2])/255;
	}
	mMipmap = new MIPMap<RGB>(width, height,data, doTri,maxAniso, wm);
}

RGB PNGImageTexture::Evaluate(const DifferentialGeometry& dg) const{
	 float s, t, dsdx, dtdx, dsdy, dtdy;
	 mMapping->Map(dg, &s, &t, &dsdx, &dtdx, &dsdy, &dtdy);
	 RGB mem = mMipmap->Lookup(s, t, dsdx, dtdx, dsdy, dtdy);
	 return mem;
}

