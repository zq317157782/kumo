/*
 * ppm.cpp
 *
 *  Created on: 2016年7月5日
 *      Author: Administrator
 */

#include <rgb.h>
#include "ppm.h"
#include "sampler.h"
#include "filter.h"
#include <fstream>

PPMFilm::PPMFilm(int xres, int yres, Filter* f, const char* file) :
		Film(xres, yres), mFilter(f), mFileName(file) {
	mPixels = new Pixel[xResolution * yResolution];

}

void PPMFilm::AddSample(const CameraSample& sample, const RGB& L) {
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

	for (int i = x0; i <=x1; ++i) {
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

void PPMFilm::WriteImage(float splatScale) {
	std::ofstream out(mFileName);
	out << "P3\n" << xResolution << " " << yResolution << "\n255\n";
	for (int j = yResolution - 1; j >= 0; --j) {
		for (int i = 0; i < xResolution; ++i) {
			Pixel p=mPixels[i + j * xResolution];
			//cout<<p.r<<p.g<<p.b<<endl;
			float invWeight=1.0/p.weightSum;
			RGB finalColor(p.r*invWeight,p.g*invWeight,p.b*invWeight);
			finalColor=Clamp(finalColor,0,1);
			out << (int) (finalColor.r * 255) << " "
					<< (int) (finalColor.g * 255) << " "
					<< (int) (finalColor.b * 255) << " ";
		}
	}
	out.close();
}
