/*
 * errfloat.h
 *
 *  Created on: 2016年11月15日
 *      Author: zhuqian
 *
 * 这个文件维护一个ErrFloat类，是PBRTV3中的EFloat类的功能，这里把名字改成ErrFloat让类名更加明确意义
 * 然后PBRTV3的书籍上的实现与其代码中的实现也有一定的差异
 */

#ifndef SRC_CORE_ERRFLOAT_H_
#define SRC_CORE_ERRFLOAT_H_

#include "kumo.h"

class ErrFloat {
private:
	float _value; //浮点数round后的值
	//float _error;//浮点数的绝对误差(real值减去round值)
	float _high;	//round值加上绝对error后的值
	float _low;	//round值减去绝对error后的值
#ifdef DEBUG_BUILD
	long double _highPrecisionValue;	//调试模式下需要计算的高精度的值
#endif
public:
	ErrFloat() {
	}
	ErrFloat(float, float err = 0.0f);
	ErrFloat(const ErrFloat&);
	ErrFloat& operator=(const ErrFloat&);
	ErrFloat operator+(ErrFloat) const;
	ErrFloat operator-(ErrFloat) const;
	ErrFloat operator*(ErrFloat) const;
	ErrFloat operator/(ErrFloat) const;
	ErrFloat operator-() const;
	bool operator==(ErrFloat) const;

	//强制转换成float
	explicit operator float() const {
		return _value;
	}

	//强制转换成double
	explicit operator double() const {
		return _value;
	}

	float UpperBound() const {
		return _high;
	}
	float LowerBound() const {
		return _low;
	}
	//Check函数是内敛函数，然后内部的实现只在DEBUG_BUILD模式有用
	//所以编译后如果不是DB模式，不会产生额外的代码
	inline void Check() const {
#ifdef DEBUG_BUILD
		//todo 对数据的检查
#endif
	}

	//几个求RAY-SHAPE相交测试用的到的友元函数
	friend ErrFloat Sqrt(ErrFloat f);
	friend ErrFloat Abs(ErrFloat f);
	friend bool Quadratic(ErrFloat A,ErrFloat B,ErrFloat C,ErrFloat* t0,ErrFloat* t1);//求解二次多项式
};
//一些float类型和ErrFloat的混合操作
inline ErrFloat operator+(float f, ErrFloat ef) {
	return ErrFloat(f) + ef;
}
inline ErrFloat operator-(float f, ErrFloat ef) {
	return ErrFloat(f) - ef;
}
inline ErrFloat operator*(float f, ErrFloat ef) {
	return ErrFloat(f) * ef;
}
inline ErrFloat operator/(float f, ErrFloat ef) {
	return ErrFloat(f) / ef;
}

inline ErrFloat Sqrt(ErrFloat f) {
	ErrFloat ret;
	ret._value = sqrt(f._value);
#ifdef DEBUG_BUILD
	ret._highPrecisionValue = sqrt(f._highPrecisionValue);
#endif
	ret._low = NextFloatDown(sqrt(f._low));
	ret._high = NextFloatUp(sqrt(f._high));
	ret.Check();
	return ret;
}

inline ErrFloat Abs(ErrFloat f) {
	//如果f的low大于0，无需任何操作
	if (f._low > 0) {
		return f;
	} else if (f._high < 0) {
		//负数的情况
		ErrFloat ret;
		ret._value = -f._value;
#ifdef DEBUG_BUILD
		ret._highPrecisionValue = -f._highPrecisionValue;
#endif
		ret._low = -f._high;
		ret._high = -f._low;
		ret.Check();
		return ret;
	} else {
		//介于负数和正数这尴尬的情况
		ErrFloat ret;
		ret._value = abs(f._value);
#ifdef DEBUG_BUILD
		ret._highPrecisionValue = abs(f._highPrecisionValue);
#endif
		ret._low=0;//貌似也只能0了
		ret._high=std::max(-f._low,f._high);//取最大的误差边界值
		ret.Check();
		return ret;
	}
}

//这样子求解二次多项式，我高中没学过啊！
inline bool Quadratic(ErrFloat A,ErrFloat B,ErrFloat C,ErrFloat* t0,ErrFloat* t1){
	//delta=B^2-4AC
	double delta=B._value*B._value-4*A._value*C._value;
	if(delta<0.0f) return false;
	double rootDelta=sqrt(delta);
	ErrFloat errRootDelta(rootDelta,MachineEpsion*rootDelta);

	ErrFloat q;
	if((float)B<0.0f){
		q=-0.5f*(B-errRootDelta);
	}
	else{
		q=-0.5f*(B+errRootDelta);
	}
	*t0=q/A;
	*t1=C/q;
	//判断顺序对不对,不对改一改，要求t0<t1
	if((float)(*t0)>(float)(*t1)) std::swap(*t0,*t1);
	return true;
}
#endif /* SRC_CORE_ERRFLOAT_H_ */
