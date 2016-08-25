/*
 * mipmap.cpp
 *
 *  Created on: 2016年8月24日
 *      Author: zhuqian
 */
#include "mipmap.h"
#include "memory.h"

template<typename T>
MIPMap<T>::MIPMap(unsigned int sres, unsigned int tres, const T *img,
		bool doTri, float maxAniso, ImageWrap wm) {
	mMaxAnisotropy = maxAniso;
	mDoTrilinear = doTri;
	mWrapMode = wm;
	T *resampledImage = nullptr; //重采样图像数据指针
	//判断是否分辨率是2的幂
	if (!IsPowerOf2(sres) || !IsPowerOf2(tres)) {
		unsigned int sPow2 = RoundUpPow2(sres);
		unsigned int tPow2 = RoundUpPow2(tres);
		ResampleWeight *sWeights = resampleWeights(sres, sPow2);//获取新的row的权重数组
		resampledImage = new T[sPow2 * tPow2];	//申请存放新的image的空间大小

		//遍历col
		for (unsigned int t = 0; t < tres; ++t) {
			//遍历新行
			for (unsigned int s = 0; s < sPow2; ++s) {
				resampledImage[t * sPow2 + s] = 0;
				//遍历权重
				for (int j = 0; j < 4; ++j) {
					int origS = sWeights[s].firstTextel + j;
					if (mWrapMode == TEXTURE_REPEAT) {
						origS = Mod(origS, sres);	//取模Repeat
					} else if (mWrapMode == TEXTURE_CLAMP) {
						origS = Clamp(origS, 0, sres - 1);	//Clamp在范围内
					}
					//把相应的贡献加入到新的texel中
					if (origS >= 0 && origS < (int) sres) {
						resampledImage[t * sPow2 + s] += sWeights[s].weight[j]
								* img[t * sres + origS];
					}
				}
			}
		}
		delete[] sWeights;
		//处理列相关的resamping操作，在前面行的resamping数据上面进行操作
		ResampleWeight *tWeights = resampleWeights(tres, tPow2);
		T *workData = new T[tPow2];
		for (unsigned int s = 0; s < sPow2; ++s) {
			for (unsigned int t = 0; t < tPow2; ++t) {
				workData[t] = 0.;
				for (int j = 0; j < 4; ++j) {
					int offset = tWeights[t].firstTexel + j;
					if (mWrapMode == TEXTURE_REPEAT) {
						offset = Mod(offset, tres);
					} else if (mWrapMode == TEXTURE_CLAMP) {
						offset = Clamp(offset, 0, tres - 1);
					}
					if (offset >= 0 && offset < (int) tres) {
						workData[t] += tWeights[t].weight[j]
								* resampledImage[offset * sPow2 + s];
					}
				}
			}
			for (unsigned int t = 0; t < tPow2; ++t) {
				resampledImage[t * sPow2 + s] = clamp(workData[t]);
			}
		}
		delete[] workData;
		delete[] tWeights;
		img = resampledImage;
		sres = sPow2;
		tres = tPow2;
	}
	//获得实际的宽和高
	mWidth = sres;
	mHeight = tres;
	//计算实际的mip-map层数
	mNumLevels = 1 + Log2Int(float(max(sres, tres)));
	mPyramid = new BlockedArray<T, 2> *[mNumLevels];	//创建一个存放所有MipMap金字塔的二位数组

	mPyramid[0] = new BlockedArray<T, 2>(sres, tres, img);	//第一层

	for (unsigned int i = 1; i < mNumLevels; ++i) {
		//从前一层获取当前层的分辨率  最小值为1X1
		unsigned int sRes = max(1u, mPyramid[i - 1]->uSize() / 2);
		unsigned int tRes = max(1u, mPyramid[i - 1]->vSize() / 2);
		mPyramid[i] = new BlockedArray<T, 2>(sRes, tRes);
		for (unsigned int t = 0; t < tRes; ++t)
			for (unsigned int s = 0; s < sRes; ++s) {
				(*mPyramid[i])(s, t) = (Texel(i - 1, 2 * s, 2 * t)
						+ Texel(i - 1, 2 * s + 1, 2 * t)
						+ Texel(i - 1, 2 * s, 2 * t + 1)
						+ Texel(i - 1, 2 * s + 1, 2 * t + 1)) * 0.25f;//(sum)/4
			}

	}
}

template<typename T>
const T & MIPMap<T>::Texel(unsigned int level, int s, int t) const {
	assert(level < mNumLevels);
	BlockedArray<T, 2>& l = *mPyramid[level];
	//根据包装模式，获取相应的texel值
	switch (mWrapMode) {
	case TEXTURE_REPEAT:
		s = Mod(s, l.uSize());
		t = Mod(t, l.vSize());
		break;
	case TEXTURE_CLAMP:
		s = Clamp(s, 0, l.uSize() - 1);
		t = Clamp(t, 0, l.vSize() - 1);
		break;
	case TEXTURE_BLACK: {
		static const T black = 0.0f;
		if (s < 0 || s >= (int) l.uSize() || t < 0 || t >= (int) l.vSize())
			return black;
		break;
	}
	}
	return l(s, t);
}

template<typename T>
T MIPMap<T>::triangle(unsigned int level, float s, float t) const {
	level = Clamp(level, 0, mNumLevels - 1);	//保证level的范围
	s = s * mPyramid[level]->uSize() - 0.5f;	//相应的在连续空间中的地址
	t = t * mPyramid[level]->vSize() - 0.5f;
	int s0 = Floor2Int(s), t0 = Floor2Int(t);	//前一个texel的索引
	float ds = s - s0, dt = t - t0;	//计算和前一个texel之间的距离
	//运用三角过滤
	return (1.f - ds) * (1.f - dt) * Texel(level, s0, t0)
			+ (1.f - ds) * dt * Texel(level, s0, t0 + 1)
			+ ds * (1.f - dt) * Texel(level, s0 + 1, t0)
			+ ds * dt * Texel(level, s0 + 1, t0 + 1);
}

//纹理查询
//width 0~1之间
template<typename T>
T MIPMap<T>::Lookup(float s, float t, float width) const {
	float level = mNumLevels - 1 + Log2(max(width, 1e-8f));
	if (level < 0)
		return triangle(0, s, t);
	else if (level >= mNumLevels - 1)
		return Texel(mNumLevels - 1, 0, 0);
	else {
		uint32_t iLevel = Floor2Int(level);
		float delta = level - iLevel;
		//两层mipmap之间按照权重混合
		return (1.f - delta) * triangle(iLevel, s, t)
				+ delta * triangle(iLevel + 1, s, t);
	}
}
