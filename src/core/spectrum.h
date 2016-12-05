/*
 * spectrum.h
 *
 *  Created on: 2016年11月25日
 *      Author: zhuqian
 */

#ifndef SRC_CORE_SPECTRUM_H_
#define SRC_CORE_SPECTRUM_H_
//人眼可见光谱波长范围380nm~780nm

//光谱相关类和函数
#include "kumo.h"

template<int numSpectrumSample>
class CoefficientSpectrum {
protected:
	Float _c[numSpectrumSample]; //用来存放光谱样本值的数组
	static constexpr int numSample = numSpectrumSample; //返回样本个数
public:

	CoefficientSpectrum(Float cc = 0.0f) {
		Assert(!isnan(cc));
		for (int i = 0; i < numSpectrumSample; ++i) {
			_c[i] = cc;
		}
	}

#ifdef DEBUG_BUILD
	//在Debug模式下才需要重写这两个函数来做断言判断
	CoefficientSpectrum(const CoefficientSpectrum& cc) {
		Assert(!cc.HasNaNs());
		for (int i = 0; i < numSpectrumSample; ++i) {
			_c[i] = cc._c[i];
		}
	}
	CoefficientSpectrum& operator=(const CoefficientSpectrum& cc) {
		Assert(!cc.HasNaNs());
		for (int i = 0; i < numSpectrumSample; ++i) {
			_c[i] = cc._c[i];
		}
		return *this;
	}
#endif

	//用来遍历样本的函数
	Float operator[](int index) const {
		Assert(index >= 0 && index < numSpectrumSample);
		return _c[index];
	}
	Float& operator[](int index) {
		Assert(index >= 0 && index < numSpectrumSample);
		return _c[index];
	}

	bool operator==(const CoefficientSpectrum& cc) const {
		for (int i = 0; i < numSpectrumSample; ++i) {
			if (cc[i] != _c[i])
				return false;
		}
		return true;
	}

	bool operator!=(const CoefficientSpectrum& cc) const {
		for (int i = 0; i < numSpectrumSample; ++i) {
			if (cc[i] == _c[i])
				return false;
		}
		return true;
	}

	bool IsBlack() const {
		for (int i = 0; i < numSpectrumSample; ++i) {
			if (_c[i] != 0.0f) {
				return false;
			}
		}
		return true;
	}

	//判断光谱样本总是否包含nan
	bool HasNaNs() const {
		for (int i = 0; i < numSpectrumSample; ++i) {
			if (isnan(_c[i]))
				return true;
		}
		return false;
	}

};

//采样的波长范围以及样本个数 400nm~700nm
static constexpr int sampledLambdaStart = 400; //nm
static constexpr int sampledLambdaEnd = 700; //nm
static constexpr int numSpectralSample = 60;

//CIE X,Y,Z曲线样本
static constexpr int numCIESample = 471;
extern const Float CIE_X[numCIESample];
extern const Float CIE_Y[numCIESample];
extern const Float CIE_Z[numCIESample];
extern const Float CIE_lambda[numCIESample];
static constexpr Float CIE_Y_integral = 106.856895;

//计算lambdaStart~lambdaEnd之间的平均能量值
Float AverageSpectrumSamples(const Float *lambda, const Float *vals, int n,
		Float lambdaStart, Float lambdaEnd);

//判断整个波长数组有没有按从小到大排列
bool SpectrumSamplesSorted(const Float *lambda, const Float *vals/*这参数压根没有用到*/,
		int n);
//根据提供的波长wl，来插值计算wl波长下的SPD的值
Float InterpolateSpectrumSamples(const Float *lambda/*样本波长*/,
		const Float *vals/*样本值*/, int n/*样本大小*/, Float wl/*提供的波长*/);

//xyz到RGB的转换
inline void XYZToRGB(const Float xyz[3], Float rgb[3]) {
	rgb[0] = 3.240479f * xyz[0] - 1.537150f * xyz[1] - 0.498535f * xyz[2];
	rgb[1] = -0.969256f * xyz[0] + 1.875991f * xyz[1] + 0.041556f * xyz[2];
	rgb[2] = 0.055648f * xyz[0] - 0.204043f * xyz[1] + 1.057311f * xyz[2];
}
//RGB到xyz的转换
inline void RGBToXYZ(const Float rgb[3], Float xyz[3]) {
	xyz[0] = 0.412453f * rgb[0] + 0.357580f * rgb[1] + 0.180423f * rgb[2];
	xyz[1] = 0.212671f * rgb[0] + 0.715160f * rgb[1] + 0.072169f * rgb[2];
	xyz[2] = 0.019334f * rgb[0] + 0.119193f * rgb[1] + 0.950227f * rgb[2];
}

//从小到大排序所有的样本
void SortSpectrumSamples(Float *lambda, Float *vals, int n);

class SampledSpectrum: public CoefficientSpectrum<numSpectralSample> {
private:
	static SampledSpectrum _X, _Y, _Z; //X,Y,Z曲线
public:
	SampledSpectrum(Float cc = 0.0) :
			CoefficientSpectrum(cc) {
	}

	static void Init() {
		// Compute XYZ matching functions for _SampledSpectrum_
		for (int i = 0; i < numSpectralSample; ++i) {
			Float wl0 = Lerp(Float(i) / Float(numSpectralSample),
					sampledLambdaStart, sampledLambdaEnd);
			Float wl1 = Lerp(Float(i + 1) / Float(numSpectralSample),
					sampledLambdaStart, sampledLambdaEnd);
			_X._c[i] = AverageSpectrumSamples(CIE_lambda, CIE_X, numCIESample,
					wl0, wl1);
			_Y._c[i] = AverageSpectrumSamples(CIE_lambda, CIE_Y, numCIESample,
					wl0, wl1);
			_Z._c[i] = AverageSpectrumSamples(CIE_lambda, CIE_Z, numCIESample,
					wl0, wl1);
		}
	}

	//用来创建SampledSpectrum的一个静态方法
	static SampledSpectrum FromSampled(const Float *lambda, const Float *v,
			int n) {
		if (!SpectrumSamplesSorted(lambda, v, n)) {
			std::vector<Float> slambda(&lambda[0], &lambda[n]);
			std::vector<Float> sv(&v[0], &v[n]);
			SortSpectrumSamples(&slambda[0], &sv[0], n); //我去，这样子取Float数组的地址
			return FromSampled(&slambda[0], &sv[0], n);
		}
		SampledSpectrum ret;
		for (int i = 0; i < numSample; ++i) {
			Float lambda0 = Lerp((Float) (i) / (Float) (numSample),
					sampledLambdaStart, sampledLambdaEnd);
			Float lambda1 = Lerp((Float) (i + 1) / (Float) (numSample),
					sampledLambdaStart, sampledLambdaEnd);
			ret._c[i] = AverageSpectrumSamples(lambda, v, n, lambda0, lambda1);
		}
		return ret;
	}

	//计算x,y,z系数
	inline void ToXYZ(Float xyz[3]) const {
		xyz[0] = xyz[1] = xyz[2] = 0;
		for (int i = 0; i < numSpectralSample; ++i) {
			xyz[0] += _X._c[i] * _c[i];
			xyz[1] += _Y._c[i] * _c[i];
			xyz[2] += _Z._c[i] * _c[i];
		}
		Float scale = Float(sampledLambdaEnd - sampledLambdaStart)
				/ Float(CIE_Y_integral * numSpectralSample);
		xyz[0] *= scale;
		xyz[1] *= scale;
		xyz[2] *= scale;
	}

	//返回y系数，同时也代表亮度
	Float y() const {
		Float yy = 0.0f;
		for (int i = 0; i < numSpectralSample; ++i) {
			yy += _Y._c[i] * _c[i];
		}
		return yy * Float(sampledLambdaEnd - sampledLambdaStart)
				/ Float(CIE_Y_integral * numSpectralSample);
	}

	void ToRGB(Float rgb[3]) const {
		Float xyz[3];
		ToXYZ(xyz);
		XYZToRGB(xyz, rgb);
	}
	//todo 实现RGB到SPD的转换
};

class RGBSpectrum: public CoefficientSpectrum<3> {
public:
	RGBSpectrum(Float cc=0.0f) :
			CoefficientSpectrum<3>(cc) {
	}
	RGBSpectrum(const CoefficientSpectrum<3>& cc) :
			CoefficientSpectrum<3>(cc) {
	}

	inline void ToRGB(Float rgb[3]) const {
		rgb[0] = _c[0];
		rgb[1] = _c[1];
		rgb[2] = _c[2];
	}
	inline void ToXYZ(Float xyz[3]) const {
		Float rgb[3];
		ToRGB(rgb);
		RGBToXYZ(rgb, xyz);
	}
	Float y() const {
		constexpr Float YWeight[3] = { 0.212671f, 0.715160f, 0.072169f };
		return YWeight[0] * _c[0] + YWeight[1] * _c[1] + YWeight[2] * _c[2];
	}

	static RGBSpectrum FromSampled(const Float *lambda, const Float *v, int n) {
		if (!SpectrumSamplesSorted(lambda, v, n)) {
			std::vector<Float> slambda(&lambda[0], &lambda[n]);
			std::vector<Float> sv(&v[0], &v[n]);
			SortSpectrumSamples(&slambda[0], &sv[0], n); //我去，这样子取Float数组的地址
			return FromSampled(&slambda[0], &sv[0], n);
		}
		//这里使用了计算xyz的积分式
		Float xyz[3];
		for (int i = 0; i < numCIESample; ++i) {
			Float val = InterpolateSpectrumSamples(lambda, v, n, CIE_lambda[i]);
			xyz[0] += val * CIE_X[i];
			xyz[1] += val * CIE_Y[i];
			xyz[2] += val * CIE_Z[i];
		}
		Float scale = Float(CIE_lambda[numCIESample - 1] - CIE_lambda[0])
				/ Float(CIE_Y_integral * numCIESample);
		xyz[0] *= scale;
		xyz[1] *= scale;
		xyz[2] *= scale;
		//转换xyz到rgb
		RGBSpectrum ret;
		XYZToRGB(xyz,ret._c);
		return ret;
	}
};

#endif /* SRC_CORE_SPECTRUM_H_ */
