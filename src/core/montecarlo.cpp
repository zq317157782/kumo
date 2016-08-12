/*
 * montecarlo.cpp
 *
 *  Created on: 2016年7月29日
 *      Author: Administrator
 */

#include "montecarlo.h"
#include "geometry.h"

Vector CosSampleHemisphere(float u1, float u2) {
	Vector ret;
	ConcentricSampleDisk(u1, u2, &ret.x, &ret.y); //这里其实可以换成任意的uniform disk sample
	ret.z = sqrtf(max(0.0f, 1.0f - ret.x * ret.x - ret.y * ret.y));
	return ret;
}

void RejectionSampleDisk(float* x, float*y, Random& rand) {
	float sx, sy;
	do {
		sx = 1 - 2 * rand.RandomFloat();
		sy = 1 - 2 * rand.RandomFloat();
	} while (sx * sx + sy * sy > 1);
	*x = sx;
	*y = sy;
}

Vector UniformSampleHemisphere(float u1, float u2) {
	float z = u1;
	float r = sqrtf(max(0.0f, 1.0f - z * z)); //这里使用max是为了做保护,防止1-z^2小于0
	float phi = 2 * M_PI * u2;
	float x = cosf(phi) * r;
	float y = sinf(phi) * r;
	return Vector(x, y, z);
}

Vector UniformSampleSphere(float u1, float u2) {
	float z = 1.0f - 2.0f * u1; //这里是和UniformSampleHemisphere唯一的区别
	float r = sqrtf(max(0.0f, 1.0f - z * z)); //这里使用max是为了做保护,防止1-z^2小于0
	float phi = 2 * M_PI * u2;
	float x = cosf(phi) * r;
	float y = sinf(phi) * r;
	return Vector(x, y, z);
}

void UniformSampleDisk(float u1, float u2, float *x, float *y) {
	float r = sqrtf(u1);
	float theta = 2 * M_PI * u2;
	*x = r * cosf(theta);
	*y = r * sinf(theta);
}

void ConcentricSampleDisk(float u1, float u2, float *dx, float *dy) {
	float r, theta;
	float sx = 2 * u1 - 1;
	float sy = 2 * u2 - 1;
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
	theta *= M_PI / 4.f;
	*dx = r * cosf(theta);
	*dy = r * sinf(theta);
}

void StratifiedSample1D(float *samp, int nSamples, Random &rand, bool jitter) {
	float invTot = 1.f / nSamples;
	for (int i = 0; i < nSamples; ++i) {
		float delta = jitter ? rand.RandomFloat() : 0.5f;
		*samp++ = (i + delta) * invTot;
	}
}

void StratifiedSample2D(float *samp, int nx, int ny, Random &rand,
		bool jitter) {
	float dx = 1.f / nx, dy = 1.f / ny;
	for (int y = 0; y < ny; ++y)
		for (int x = 0; x < nx; ++x) {
			float jx = jitter ? rand.RandomFloat() : 0.5f;
			float jy = jitter ? rand.RandomFloat() : 0.5f;
			*samp++ = (x + jx) * dx;
			*samp++ = (y + jy) * dy;
		}
}

void LatinHypercube(float *samples, unsigned int nSamples, unsigned int nDim,
		Random &rng) {
	// Generate LHS samples along diagonal
	//沿着对角线生成LHS样本
	float delta = 1.f / nSamples;
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
