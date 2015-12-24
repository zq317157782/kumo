//
// Created by 诸谦 on 15/12/24.
//

#ifndef RAYTRACER_VECTOR2_H
#define RAYTRACER_VECTOR2_H

/**
 * 二维向量
 */
class Vector2 {
public:
    double x,y;
    Vector2(const double& _x,const double& _y);
    Vector2(const double& _all=0);

    Vector2 operator+(const Vector2& vec) const;
    Vector2 operator+(const double& d) const;
    Vector2 operator-(const Vector2& vec) const;
    Vector2 operator-(const double& d) const;
    Vector2 operator-() const;
    Vector2 operator*(double v) const;
    Vector2 operator*(const Vector2& vec) const;
    Vector2& operator+=(const Vector2& vec);
    Vector2& operator-=(const Vector2& vec);
    Vector2& operator*=(const Vector2& vec);

    double modulo() const;      //取模运算
    Vector2& normalize(); //标准化本向量
    Vector2 unit() const;       //返回单位向量
    double dot(const Vector2& vec) const;
    double distance2(const Vector2& b) const;

    friend Vector2 operator+(const double& a,const Vector2& b);
    friend Vector2 operator*(const double& a,const Vector2& b);

    bool operator==(const Vector2& vec) const;
    bool operator!=(const Vector2& vec) const;
};


#endif //RAYTRACER_VECTOR2_H
