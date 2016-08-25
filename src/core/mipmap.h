/*
 * mipmap.h
 *
 *  Created on: 2016年8月24日
 *      Author: zhuqian
 */

#ifndef SRC_CORE_MIPMAP_H_
#define SRC_CORE_MIPMAP_H_
#include "global.h"
#include "texture.h"
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
public:
	MIPMap(unsigned int sres, unsigned int tres, const T *img, bool doTri,
			float maxAniso, ImageWrap wm);

	//获取某一个Mip_map层级中的某个texel的值
	const T &Texel(unsigned int level, int s, int t) const;

	T Lookup(float s, float t, float width = 0.f) const;			//纹理查询函数

	unsigned int  Width() const {
		return mWidth;
	}
	unsigned int Height() const {
		return mHeight;
	}
	unsigned int Levels() const {
		return mNumLevels;
	}

};

#endif /* SRC_CORE_MIPMAP_H_ */
