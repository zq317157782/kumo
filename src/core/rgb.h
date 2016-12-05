//
// Created by 诸谦 on 15/12/24.
//

#ifndef RAYTRACER_RGB_H
#define RAYTRACER_RGB_H
#include <kumo.h>
/**
 * RGB颜色
 */
class RGB {
public:
	Float r, g, b;
	RGB(Float _r, Float _g, Float _b);
	RGB(Float _all = 0);

	RGB operator+(const RGB& c) const;
	RGB operator+(Float f) const;
	RGB operator-(const RGB& c) const;
	RGB operator-(Float d) const;
	RGB operator*(Float v) const;
	RGB operator*(const RGB& c) const;
	RGB& operator+=(const RGB& c);
	RGB& operator-=(const RGB& c);
	RGB& operator*=(const RGB& c);

	RGB operator/(Float div) const;
	RGB operator/(const RGB& rgb) const;

	friend RGB operator+(Float a, const RGB& b);
	friend RGB operator*(Float a, const RGB& b);

	bool operator==(const RGB& c) const;
	bool operator!=(const RGB& c) const;

	//RGB clamp();

	Float luminance() const {
		const Float YWeight[3] = { 0.212671f, 0.715160f, 0.072169f };
		return YWeight[0] * r + YWeight[1] * g + YWeight[2] * b;
	}

	bool IsBlack() const{
		return r == 0 && g == 0 && b == 0;
	}
};

inline RGB Gamma(const RGB& rgb, Float a, Float gamma) {
	RGB ret;
	ret.r = a * powf(rgb.r, gamma);
	ret.g = a * powf(rgb.g, gamma);
	ret.b = a * powf(rgb.b, gamma);
	return ret;
}

inline RGB Clamp(const RGB& rgb,Float minV,Float maxV){
	 return RGB(std::max(minV, std::min(maxV,rgb.r)), std::max(minV, std::min(maxV,rgb.g)), std::max(minV, std::min(maxV,rgb.b)));
}

#endif //RAYTRACER_RGB_H
