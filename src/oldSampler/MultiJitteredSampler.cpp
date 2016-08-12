//
// Created by 诸谦 on 15/12/24.
//

#include <math.h>

#include "../oldSampler/MultiJitteredOldSampler.h"

MultiJitteredSampler::MultiJitteredSampler(int _sampleNum,int _setNum) : OldSampler(_sampleNum, _setNum) {
    generateSamples();
}

void MultiJitteredSampler::generateSamples() {
    int n=(int)sqrt((float)mSampleNum);
    float subcell_width=1.0/((float)mSampleNum);

    Point fill_point;
    for(int j=0;j<mSampleNum*mSetNum;++j)
        mSamples.push_back(fill_point);

    for(int p=0;p<mSetNum;++p)
        for(int i=0;i<n;++i)
            for(int j=0;j<n;j++){
                mSamples[i*n+j+p*mSampleNum].x=(i * n + j) * subcell_width + drand48()*subcell_width;
                mSamples[i*n+j+p*mSampleNum].y=(i * n + j) * subcell_width + drand48()*subcell_width;
            }


    for(int p=0;p<mSetNum;++p)
        for(int i=0;i<n;++i)
            for(int j=0;j<n;j++){
                int k=(rand()%(n-j))+j;
                double t =mSamples[i*n+j+p*mSampleNum].x;
                mSamples[i * n + j + p * mSampleNum].x = mSamples[i * n + k + p * mSampleNum].x;
                mSamples[i * n + k + p * mSampleNum].x = t;
            }


    for(int p=0;p<mSetNum;++p)
        for(int i=0;i<n;++i)
            for(int j=0;j<n;j++){
                int k=(rand()%(n-j))+j;
                float t =mSamples[i*n+j+p*mSampleNum].y;
                mSamples[i * n + j + p * mSampleNum].y = mSamples[i * n + k + p * mSampleNum].y;
                mSamples[i * n + k + p * mSampleNum].y = t;
            }
}
