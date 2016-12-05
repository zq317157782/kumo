/*
 * mipmap.h
 *
 *  Created on: 2016年8月24日
 *      Author: zhuqian
 */

#ifndef SRC_CORE_MIPMAP_H_
#define SRC_CORE_MIPMAP_H_
#include <rgb.h>
#include "texture.h"
#include "memory.h"
#include "seidennki.h"

//纹理包装模式
//The ImageWrap enumerant, passed to the MIPMap constructor, specifies the desired behav-
//ior when the supplied texture coordinates are not in the legal [0, 1] range.
typedef enum {
	TEXTURE_REPEAT, TEXTURE_BLACK, TEXTURE_CLAMP
} ImageWrap;

//MIP_MAP结构
template<typename T>
class MIPMap {
private:
	bool mDoTrilinear; //三线性采样
	float mMaxAnisotropy; //各项异性相关
	ImageWrap mWrapMode; //包装模式
	unsigned int mWidth, mHeight, mNumLevels;
	BlockedArray<T, 2> **mPyramid;
	//用来记录每个新的texel相应的旧的texel的权重
	struct ResampleWeight {
		int firstTexel;
		float weight[4];
	};

	//返回一个权重数组 默认使用BOX Filter
	ResampleWeight *resampleWeights(unsigned int oldres, unsigned int newres) {
		assert(newres >= oldres);
		ResampleWeight *wt = new ResampleWeight[newres]; //生成相应的权重数组
		float filterwidth = 2.0f; //BOX过滤器的Range
		//计算新的texel的权重
		for (unsigned int i = 0; i < newres; ++i) {
			float center = (i + 0.5f) * oldres / newres;//计算新的texel在旧环境下的位置(example: i==0说明新texel在连续空间中的位置是0.5,所以要加0.5)
			//找到第一个旧texel,这里加0.5是因为一个只有filter的range超过这个texel的一半才能算采样到这个texel,所以加0.5来修正FloorToInt
			wt[i].firstTexel = Floor2Int((center - filterwidth) + 0.5f);
			//开始计算权重
			for (int j = 0; j < 4; ++j) {
				float pos = wt[i].firstTexel + j + 0.5f;//计算4个旧texel在连续空间中的位置
				wt[i].weight[j] = Lanczos((pos - center) / filterwidth, 2);	//计算权重
			}
			//下面是标准化权重，为了不让新生成的图像比原图暗或者亮
			float invSumWts = 1.f
					/ (wt[i].weight[0] + wt[i].weight[1] + wt[i].weight[2]
							+ wt[i].weight[3]);
			for (unsigned int j = 0; j < 4; ++j)
				wt[i].weight[j] *= invSumWts;
		}
		return wt;
	}

	//三角过滤
	T triangle(unsigned int level, float s, float t) const;

	float clamp(float v) {
		return Clamp(v, 0.f, INFINITY);
	}
	//TODO 这里RGB没有Clamp函数
	RGB clamp(const RGB &v) {
		return v;
	}
public:
	MIPMap() {
		mPyramid = nullptr;
		mWidth = mHeight = mNumLevels = 0;
	}
	MIPMap(unsigned int sres, unsigned int tres, const T *img, bool doTri,
			float maxAniso, ImageWrap wm);

	//获取某一个Mip_map层级中的某个texel的值
	const T &Texel(unsigned int level, int s, int t) const;

	T Lookup(float s, float t, float width = 0.f) const;			//纹理查询函数
	T Lookup(float s, float t, float ds0, float dt0, float ds1,
			float dt1) const;

	unsigned int Width() const {
		return mWidth;
	}
	unsigned int Height() const {
		return mHeight;
	}
	unsigned int Levels() const {
		return mNumLevels;
	}

};

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
					int origS = sWeights[s].firstTexel + j;
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

template<typename T>
T MIPMap<T>::Lookup(float s, float t, float ds0, float dt0, float ds1,
		float dt1) const {
	if (mDoTrilinear) {
		T val = Lookup(s, t,
				2.0f* max(max(fabsf(ds0), fabsf(dt0)),max(fabsf(ds1), fabsf(dt1))));
		return val;
	}
	assert(false);
}

#endif /* SRC_CORE_MIPMAP_H_ */
