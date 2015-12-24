//
// Created by 诸谦 on 15/12/23.
//

#ifndef RAYTRACER_VECTOR3_H
#define RAYTRACER_VECTOR3_H

/**
 *三维向量类
 */
class Vector3{
public:
    double x,y,z;
    Vector3(const double& _x,const double& _y,const double& _z);
    Vector3(const double& _all=0);

    Vector3 operator+(const Vector3& vec) const;
    Vector3 operator+(const double& d) const;
    Vector3 operator-(const Vector3& vec) const;
    Vector3 operator-(const double& d) const;
    Vector3 operator-() const;
    Vector3 operator*(double v) const;
    Vector3 operator*(const Vector3& vec) const;
    Vector3& operator+=(const Vector3& vec);
    Vector3& operator-=(const Vector3& vec);
    Vector3& operator*=(const Vector3& vec);

    double modulo() const;      //取模运算
    Vector3& normalize(); //标准化本向量
    Vector3 unit() const;       //返回单位向量
    double dot(const Vector3& vec) const;
    Vector3 cross(const Vector3& vec) const;    //叉积公式：u x v = { u2v3-v2u3 , u3v1-v3u1 , u1v2-u2v1 }
    double distance2(const Vector3& b) const;

    friend Vector3 operator+(const double& a,const Vector3& b);
    friend Vector3 operator*(const double& a,const Vector3& b);

    bool operator==(const Vector3& vec) const;
    bool operator!=(const Vector3& vec) const;
};


#endif //RAYTRACER_VECTOR3_H
