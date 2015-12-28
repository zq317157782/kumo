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
    RGB(const float& _r,const float& _g,const float& _b);
    RGB(const float& _all=0);

    RGB operator+(const RGB& c) const;
    RGB operator+(const float& f) const;
    RGB operator-(const RGB& c) const;
    RGB operator-(const float& d) const;
    RGB operator*(float v) const;
    RGB operator*(const RGB& c) const;
    RGB& operator+=(const RGB& c);
    RGB& operator-=(const RGB& c);
    RGB& operator*=(const RGB& c);

    RGB operator/(const double div) const;

    friend RGB operator+(const float& a,const RGB& b);
    friend RGB operator*(const float& a,const RGB& b);

    bool operator==(const RGB& c) const;
    bool operator!=(const RGB& c) const;

};


#endif //RAYTRACER_RGB_H
