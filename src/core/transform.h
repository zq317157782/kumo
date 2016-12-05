//
// Created by 诸谦 on 16/6/9.
//

#ifndef SRC_TRANSFORM_H
#define SRC_TRANSFORM_H
#include "geometry.h"
#include "seidennki.h"

//求解2x2线性方程组 AX=b
bool SolveLinearSystem2x2(const float A[2][2],
        const float B[2], float *x0, float *x1);

struct Matrix4X4{
    float m[4][4];

    Matrix4X4(){
        m[0][0]=m[1][1]=m[2][2]=m[3][3]=1.0;
        m[0][1]=m[0][2]=m[0][3]=0.0;
        m[1][0]=m[1][2]=m[1][3]=0.0;
        m[2][0]=m[2][1]=m[2][3]=0.0;
        m[3][0]=m[3][1]=m[3][2]=0.0;
    }

    Matrix4X4(float mm[4][4]);

    Matrix4X4(float t00, float t01, float t02, float t03,
              float t10, float t11, float t12, float t13,
              float t20, float t21, float t22, float t23,
              float t30, float t31, float t32, float t33){
        m[0][0] = t00; m[0][1] = t01; m[0][2] = t02; m[0][3] = t03;
        m[1][0] = t10; m[1][1] = t11; m[1][2] = t12; m[1][3] = t13;
        m[2][0] = t20; m[2][1] = t21; m[2][2] = t22; m[2][3] = t23;
        m[3][0] = t30; m[3][1] = t31; m[3][2] = t32; m[3][3] = t33;
    }


    bool operator==(const Matrix4X4& mat)const{
        for(int i=0;i<4;++i)
            for(int j=0;j<4;++j)
                if(m[i][j]!=mat.m[i][j]) return false;
        return true;
    }

    bool operator!=(const Matrix4X4& mat) const{
        for(int i=0;i<4;++i)
            for(int j=0;j<4;++j)
                if(m[i][j]!=mat.m[i][j]) return true;
        return false;
    }
    friend Matrix4X4 Transpose(const Matrix4X4 &mm);
    //求逆
    friend Matrix4X4 Inverse(const Matrix4X4 &mm);

    static Matrix4X4 Mul(const Matrix4X4 &mm1, const Matrix4X4 &mm2){
        Matrix4X4 mat;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                mat.m[i][j] = mm1.m[i][0] * mm2.m[0][j] +
                            mm1.m[i][1] * mm2.m[1][j] +
                            mm1.m[i][2] * mm2.m[2][j] +
                            mm1.m[i][3] * mm2.m[3][j];
        return mat;
    }
};


//变换
class Transform{
private:
    Matrix4X4 m,invM;
public:
    Transform(){

    }

    Transform(float mm[4][4]){
        m=Matrix4X4(mm);
        invM= Inverse(m);//取逆
    }

    Transform(const Matrix4X4& mm):m(mm),invM(Inverse(mm)){

    }

    Transform(const Matrix4X4& mm,const Matrix4X4& invMm):m(mm),invM(invMm){

    }


    Vector operator()(const Vector& v) const;
    void operator()(const Vector& v,Vector * rv) const;
    Point operator()(const Point& p) const;
    void operator()(const Point& p,Point* rp) const;
    Ray operator()(const Ray& r) const;
    void operator()(const Ray& r,Ray * rr) const;
    RayDifferential operator()(const RayDifferential &r) const;
    void operator()(const RayDifferential &r, RayDifferential *rt) const;
    Normal operator()(const Normal& n) const;
    void operator()(const Normal& n,Normal* normal) const;
    BBox operator()(const BBox &b) const;
    bool SwapsHandedness() const;//判断变换有没有改变坐标系系统(左手=>右手 OR 右手=>左手)

    //判断是否是单位矩阵
    bool IsIdentity() const {
        return (m.m[0][0] == 1.f && m.m[0][1] == 0.f &&
                m.m[0][2] == 0.f && m.m[0][3] == 0.f &&
                m.m[1][0] == 0.f && m.m[1][1] == 1.f &&
                m.m[1][2] == 0.f && m.m[1][3] == 0.f &&
                m.m[2][0] == 0.f && m.m[2][1] == 0.f &&
                m.m[2][2] == 1.f && m.m[2][3] == 0.f &&
                m.m[3][0] == 0.f && m.m[3][1] == 0.f &&
                m.m[3][2] == 0.f && m.m[3][3] == 1.f);
    }

    friend Transform Inverse(const Transform& t);
    Transform operator*(const Transform& tran) const;
};

inline Transform Inverse(const Transform& t){
	return Transform(t.invM,t.m);
}

Transform Translate(const Vector &delta);
Transform Scale(float x,float y,float z);


Transform RotateX(float angle);
Transform RotateY(float angle);
Transform RotateZ(float angle);

Transform Rotate(float angle, const Vector &axis);

Transform Orthographic(float znear, float zfar);//正交变换
Transform Perspective(float fov, float znear, float zfar);//透视变换 fov:field of view
#endif //SRC_TRANSFORM_H
