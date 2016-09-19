/*
 * png.cpp
 *
 *  Created on: 2016年8月11日
 *      Author: zhuqian
 */

#include "png.h"
#include "sampler.h"
#include "filter.h"
#include "RGB.h"
#include <fstream>
#include "../thrid/lodepng/lodepng.h"

PNGFilm::PNGFilm(int xres, int yres, Filter* f, const char* file) :
		Film(xres, yres), mFilter(f), mFileName(file) {
	mPixels = new Pixel[xResolution * yResolution];
}

void PNGFilm::AddSample(const CameraSample& sample, const RGB& L) {
	float dimageX = sample.imageX - 0.5f;
	float dimageY = sample.imageY - 0.5f;
	int x0 = Ceil2Int(dimageX - mFilter->xWidth);
	int x1 = Floor2Int(dimageX + mFilter->xWidth);
	int y0 = Ceil2Int(dimageY - mFilter->yWidth);
	int y1 = Floor2Int(dimageY + mFilter->yWidth);
	x0 = max(x0, 0);
	x1 = min(x1, xResolution - 1);
	y0 = max(y0, 0);
	y1 = min(y1, yResolution - 1);

	//cout<<x0<<x1<<y0<<y1<<endl;

	for (int i = x0; i <= x1; ++i) {
		for (int j = y0; j <= y1; ++j) {
			float weight = mFilter->Evaluate(i - sample.imageX,
					j - sample.imageY);
			mPixels[i + j * xResolution].r += weight * L.r;
			mPixels[i + j * xResolution].g += weight * L.g;
			mPixels[i + j * xResolution].b += weight * L.b;
			mPixels[i + j * xResolution].weightSum += weight;

		}
	}
}

void PNGFilm::WriteImage(float splatScale) {

	vector<unsigned char> image;
	for (int j =0;j<yResolution;++j) {
		for (int i = 0; i < xResolution; ++i) {
			Pixel p = mPixels[i + j * xResolution];
			float invWeight = p.weightSum!=0?1.0 / p.weightSum:1;
			RGB finalColor(p.r * invWeight, p.g * invWeight, p.b * invWeight);
			finalColor=Gamma(finalColor,1,0.45);
			finalColor=finalColor.clamp();
			image.push_back((int) (finalColor.r * 255));
			image.push_back((int) (finalColor.g * 255));
			image.push_back((int) (finalColor.b * 255));
			image.push_back((int) (255));
		}
	}
	unsigned error = lodepng::encode(mFileName,image,xResolution,
			yResolution);
	if (error)
		std::cout << "encoder error " << error << ": "
				<< lodepng_error_text(error) << std::endl;

}

