//
// Created by 诸谦 on 16/6/12.
//

#ifndef RAYTRACER_TEST_H
#define RAYTRACER_TEST_H


#include "gtest/gtest.h"
#include "geometry.h"
#include "transform.h"

TEST(Transform,checkScale){
    Vector v(1,1,1);
    Transform tran=Scale(0.5f,0.3f,0.2f);
    Vector v2= tran(v);
    ASSERT_EQ(v2.x,0.5f);
    ASSERT_EQ(v2.y,0.3f);
    ASSERT_EQ(v2.z,0.2f);
}


TEST(Transform,checkRotate){

    Transform tranX=RotateX(45);
    Vector v(1,0,0);
    v=tranX(v);
    ASSERT_EQ(v.x,1);
    ASSERT_EQ(v.y,0);
    ASSERT_EQ(v.z,0);

    Vector v2(0,1,0);
    v2=tranX(v2);
    ASSERT_EQ(v2.x,0);
    ASSERT_EQ(v2.y,sinf(Radians(45)));
    ASSERT_EQ(v2.z,cosf(Radians(45)));
}


#endif //RAYTRACER_TEST_H
