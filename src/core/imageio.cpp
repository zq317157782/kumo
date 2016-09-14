/*
 * imageio.cpp
 *
 *  Created on: 2016年9月14日
 *      Author: zhuqian
 */
#include "imageio.h"
#include "../thrid/lodepng/lodepng.h"
#include "RGB.h"

RGB *ReadPNGImage(const string &name, int *xSize, int *ySize) {
	std::vector<unsigned char> image;
	unsigned int width;
	unsigned int height;
	unsigned error = lodepng::decode(image, width, height, name);
	if (error) {
		std::cerr << "decoder error " << error << ": "
				<< lodepng_error_text(error) << std::endl;
		exit(-1);
	}
	*xSize = width;
	*ySize = height;
	RGB* data = new RGB[width * height];
	for (int i = 0; i < width * height; ++i) {
		data[i].r = ((float) image[i * 4]) / 255;
		data[i].g = ((float) image[i * 4 + 1]) / 255;
		data[i].b = ((float) image[i * 4 + 2]) / 255;
	}
	return data;
}

RGB *ReadImage(const string &name, int *xSize, int *ySize) {
	if (name.size() >= 5) {
		unsigned int suffixOffset = name.size() - 4;
		if (!strcmp(name.c_str() + suffixOffset, ".png")
				|| !strcmp(name.c_str() + suffixOffset, ".PNG")) {

			return ReadPNGImage(name, xSize, ySize);
		}
		cerr
				<< ("Unable to load image stored in format \"%s\" for filename \"%s\". "
						"Returning a constant grey image instead.",
						strrchr(name.c_str(), '.') ?
								(strrchr(name.c_str(), '.') + 1) : "(unknown)", name.c_str())
				<< endl;
		RGB *ret = new RGB[1];
		ret[0] = 0.5f;
		*xSize = *ySize = 1;
	}
}

