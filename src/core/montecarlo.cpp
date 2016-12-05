/*
 * montecarlo.cpp
 *
 *  Created on: 2016年7月29日
 *      Author: Administrator
 */

#include "montecarlo.h"
#include "geometry.h"

Vector CosSampleHemisphere(Float u1, Float u2) {
	Vector ret;
	ConcentricSampleDisk(u1, u2, &ret.x, &ret.y); //这里其实可以换成任意的uniform disk sample
	ret.z = sqrtf(max(0.0f, 1.0f - ret.x * ret.x - ret.y * ret.y));
	return ret;
}

void RejectionSampleDisk(Float* x, Float*y, Random& rand) {
	Float sx, sy;
	do {
		sx = 1 - 2 * rand.RandomFloat();
		sy = 1 - 2 * rand.RandomFloat();
	} while (sx * sx + sy * sy > 1);
	*x = sx;
	*y = sy;
}

Vector UniformSampleHemisphere(Float u1, Float u2) {
	Float z = u1;
	Float r = sqrtf(max(0.0f, 1.0f - z * z)); //这里使用max是为了做保护,防止1-z^2小于0
	Float phi = 2 * Pi * u2;
	Float x = cosf(phi) * r;
	Float y = sinf(phi) * r;
	return Vector(x, y, z);
}

Vector UniformSampleSphere(Float u1, Float u2) {
	Float z = 1.0f - 2.0f * u1; //这里是和UniformSampleHemisphere唯一的区别
	Float r = sqrtf(max(0.0f, 1.0f - z * z)); //这里使用max是为了做保护,防止1-z^2小于0
	Float phi = 2 * Pi * u2;
	Float x = cosf(phi) * r;
	Float y = sinf(phi) * r;
	return Vector(x, y, z);
}

void UniformSampleDisk(Float u1, Float u2, Float *x, Float *y) {
	Float r = sqrtf(u1);
	Float theta = 2 * Pi * u2;
	*x = r * cosf(theta);
	*y = r * sinf(theta);
}

void ConcentricSampleDisk(Float u1, Float u2, Float *dx, Float *dy) {
	Float r, theta;
	Float sx = 2 * u1 - 1;
	Float sy = 2 * u2 - 1;
	if (sx == 0.0 && sy == 0.0) {
		*dx = 0.0;
		*dy = 0.0;
		return;
	}
	if (sx >= -sy) {
		if (sx > sy) {
			r = sx;
			if (sy > 0.0)
				theta = sy / r;
			else
				theta = 8.0f + sy / r;
		} else {
			r = sy;
			theta = 2.0f - sx / r;
		}
	} else {
		if (sx <= sy) {
			r = -sx;
			theta = 4.0f - sy / r;
		} else {
			r = -sy;
			theta = 6.0f + sx / r;
		}
	}
	theta *= Pi / 4.f;
	*dx = r * cosf(theta);
	*dy = r * sinf(theta);
}

void StratifiedSample1D(Float *samp, int nSamples, Random &rand, bool jitter) {
	Float invTot = 1.f / nSamples;
	for (int i = 0; i < nSamples; ++i) {
		Float delta = jitter ? rand.RandomFloat() : 0.5f;
		*samp++ = (i + delta) * invTot;
	}
}

void StratifiedSample2D(Float *samp, int nx, int ny, Random &rand,
		bool jitter) {
	Float dx = 1.f / nx, dy = 1.f / ny;
	for (int y = 0; y < ny; ++y)
		for (int x = 0; x < nx; ++x) {
			Float jx = jitter ? rand.RandomFloat() : 0.5f;
			Float jy = jitter ? rand.RandomFloat() : 0.5f;
			*samp++ = (x + jx) * dx;
			*samp++ = (y + jy) * dy;
		}
}

void LatinHypercube(Float *samples, unsigned int nSamples, unsigned int nDim,
		Random &rng) {
	// Generate LHS samples along diagonal
	//沿着对角线生成LHS样本
	Float delta = 1.f / nSamples;
	for (unsigned int i = 0; i < nSamples; ++i)
		for (unsigned int j = 0; j < nDim; ++j)
			samples[nDim * i + j] = (i + (rng.RandomFloat())) * delta;

	// Permute LHS samples in each dimension
	//在各个维度上重新排列LHS样本
	for (unsigned int i = 0; i < nDim; ++i) {
		for (unsigned int j = 0; j < nSamples; ++j) {
			unsigned int other = j + (rng.RandomUInt() % (nSamples - j));
			swap(samples[nDim * j + i], samples[nDim * other + i]);
		}
	}
}

int LDPixelSampleFloatsNeeded(const Sample* sample, int numPixelSamples) {
	int n = 4;	//2 lens + 2 image
	for (unsigned int i = 0; i < sample->n1D.size(); ++i) {
		n += sample->n1D[i];
	}
	for (unsigned int i = 0; i < sample->n2D.size(); ++i) {
		n += sample->n2D[i] * 2;
	}
	return numPixelSamples * n;
}

//采样LD样本
void LDPixelSample(int xPos, int yPos, int numPixelSamples, Sample* samples,
		Float * buf, Random rand) {
	//分配不同的指针指向相应的内存空间
	Float* imageSamples = buf;
	buf += 2 * numPixelSamples;
	Float* lenSamples = buf;
	buf += 2 * numPixelSamples;

	//开始分配积分器样本
	unsigned int count1D = samples[0].n1D.size();
	unsigned int count2D = samples[0].n2D.size();
	const unsigned int *n1D = &samples[0].n1D[0];
	const unsigned int *n2D = &samples[0].n2D[0];
	//指向积分器样本的指针
	Float **oneDSamples = ALLOCA(Float*, count1D);
	Float **twoDSamples = ALLOCA(Float*, count2D);
	for (unsigned int i = 0; i < count1D; ++i) {
		oneDSamples[i] = buf;
		buf += n1D[i] * numPixelSamples;
	}
	for (unsigned int i = 0; i < count2D; ++i) {
		twoDSamples[i] = buf;
		buf += n2D[i] * numPixelSamples * 2;
	}

	//正式生成采样点
	LDShuffleScrambled2D(1, numPixelSamples, imageSamples, rand);
	LDShuffleScrambled2D(1, numPixelSamples, lenSamples, rand);
	for (unsigned int i = 0; i < count1D; ++i) {
		LDShuffleScrambled1D(n1D[i], numPixelSamples, oneDSamples[i], rand);
	}
	for (unsigned int i = 0; i < count2D; ++i) {
		LDShuffleScrambled2D(n2D[i], numPixelSamples, twoDSamples[i], rand);
	}

	for (int i = 0; i < numPixelSamples; ++i) {
		samples[i].imageX = xPos + imageSamples[i * 2];
		samples[i].imageY = yPos + imageSamples[i * 2 + 1];

		samples[i].lensU = lenSamples[2 * i];
		samples[i].lensV = lenSamples[2 * i + 1];

		for (unsigned int j = 0; j < count1D; ++j) {
			int startSamp = n1D[j] * i;
			for (unsigned int k = 0; k < n1D[j]; ++k) {
				samples[i].oneD[j][k] = oneDSamples[j][startSamp + k];
			}
		}

		for (unsigned int j = 0; j < count2D; ++j) {
			int startSamp = n2D[j] * i*2;
			for (unsigned int k = 0; k < n2D[j]*2; ++k) {
				samples[i].twoD[j][k] = twoDSamples[j][startSamp + k];
			}
		}

	}
}
