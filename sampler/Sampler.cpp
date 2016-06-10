//
// Created by 诸谦 on 15/12/24.
//

#include <math.h>
#include <assert.h>
#include "Sampler.h"
const int Sampler::DEFAULT_SET_NUM=83;

Sampler::Sampler( int _sampleNum, int _setNum):mSampleNum(_sampleNum),mSetNum(_setNum),mJump(0),mCount(0),mIsDiskMapped(false),mIsHemiMapped(false){
    assert(mSampleNum>0);
    assert(mSetNum>0);
}

Vector2 Sampler::sampleUnitSquare(){
    if(mCount%mSampleNum==0){
        mJump=(rand()%mSetNum)*mSampleNum;
    }
    return (mSamples[(mJump+mCount++)%(mSampleNum*mSetNum)]);
}

Vector2 Sampler::sampleUnitDisk(){
    assert(mIsDiskMapped==true);
    if(mCount%mSampleNum==0){
        mJump=(rand()%mSetNum)*mSampleNum;
    }
    return (mSamplesDisk[(mJump+mCount++)%(mSampleNum*mSetNum)]);
}

Vector Sampler::sampleHemi(){
    assert(mIsHemiMapped==true);
    if(mCount%mSampleNum==0){
        mJump=(rand()%mSetNum)*mSampleNum;
        mCount=0;
    }
    return (mSamplesHemi[(mJump+mCount++)%(mSampleNum*mSetNum)]);
}


void Sampler::mapSamples2UnitDisk(){
    mIsDiskMapped=true;
    int size=mSamples.size();
    float r,phi;//极坐标
    Vector2 sp;//sample point
    mSamplesDisk.reserve(size);

    for(int j=0;j<size;++j){
        sp.x=2.0*mSamples[j].x-1.0;
        sp.y=2.0*mSamples[j].y-1.0;

        if(sp.x>-sp.y){
            if(sp.x>sp.y){
                r=sp.x;
                phi=sp.y/sp.x;
            }else{
                r=sp.y;
                phi=2-sp.x/sp.y;
            }
        }else{
            if(sp.x<sp.y){
                r=-sp.x;
                phi=4+sp.y/sp.x;
            }
            else{
                r=-sp.y;
                if(sp.y!=0.0)
                    phi=6-sp.x/sp.y;
                else
                    phi=0.0;
            }
        }
        phi*=M_PI/4.0;
        mSamplesDisk[j].x=r*cos(phi);
        mSamplesDisk[j].y=r*sin(phi);
    }
}


void Sampler::mapSamples2Hemisphere(const float& e){
    mIsHemiMapped=true;
    int size=mSamples.size();
    mSamplesHemi.reserve(mSampleNum*mSetNum);
    for(int j=0;j<size;++j){
        float cos_phi=cos(2*M_PI*mSamples[j].x);
        float sin_phi=sin(2*M_PI*mSamples[j].x);
        float cos_theta=pow((1.0-mSamples[j].y),1.0/(e+1.0));
        float sin_theta=sqrt(1.0-cos_theta*cos_theta);
        float pu=sin_theta*cos_phi;
        float pv=sin_theta*sin_phi;
        float pw=cos_theta;
        mSamplesHemi.push_back(Vector(pu, pv, pw));
    }
}

int Sampler::getSampleNum() const {
    return mSampleNum;
}

int Sampler::getSetNum() const {
    return mSetNum;
}
