//
// Created by 诸谦 on 16/6/12.
//

#ifndef RAYTRACER_TEST_H
#define RAYTRACER_TEST_H

#ifdef UNIT_TEST

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

    Vector v3(0,0,1);
    Transform tranA=Rotate(90.0,Vector(0,1,0));
    v3=tranA(v3);
    ASSERT_EQ(v3.x,1);
    ASSERT_EQ(v3.y,0);
    ASSERT_TRUE( fabsf(v3.z)< 0.0001f);
}
#include "../sampler/randomSampler.h"
#include "random.h"
#include "sampler.h"
TEST(RandomSampler,ctor){
	RandomSampler rs(0,32,0,32,6);
	Sample *s=new Sample(&rs,nullptr,nullptr);
	Random ran;
	rs.GetMoreSamples(s,ran);
	rs.GetMoreSamples(s,ran);
	rs.GetMoreSamples(s,ran);
	rs.GetMoreSamples(s,ran);
	rs.GetMoreSamples(s,ran);
	rs.GetMoreSamples(s,ran);
//	rs.GetMoreSamples(s,ran);
//	rs.GetMoreSamples(s,ran);
	ASSERT_EQ(s->imageX,0);
}

#endif

#endif //RAYTRACER_TEST_H
