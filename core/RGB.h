//
// Created by 诸谦 on 15/12/24.
//

#ifndef RAYTRACER_RGB_H
#define RAYTRACER_RGB_H

/**
 * RGB颜色
 */
class RGB {
public:
    float r,g,b;
    RGB(float _r,float _g,float _b);
    RGB(float _all=0);

    RGB operator+(const RGB& c) const;
    RGB operator+(float f) const;
    RGB operator-(const RGB& c) const;
    RGB operator-(float d) const;
    RGB operator*(float v) const;
    RGB operator*(const RGB& c) const;
    RGB& operator+=(const RGB& c);
    RGB& operator-=(const RGB& c);
    RGB& operator*=(const RGB& c);

    RGB operator/(float div) const;
    RGB operator/(const RGB& rgb) const;

    friend RGB operator+( float a,const RGB& b);
    friend RGB operator*( float a,const RGB& b);

    bool operator==(const RGB& c) const;
    bool operator!=(const RGB& c) const;

    RGB clamp();


    float y() const {
            const float YWeight[3] = { 0.212671f, 0.715160f, 0.072169f };
            return YWeight[0] * r + YWeight[1] * g + YWeight[2] * b;
    }
};


#endif //RAYTRACER_RGB_H
