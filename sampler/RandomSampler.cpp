//
// Created by 诸谦 on 15/12/24.
//

#include <math.h>
#include "RandomSampler.h"

RandomSampler::RandomSampler( int _sampleNum,  int _setNum) : Sampler(_sampleNum, _setNum) {
    generateSamples();
}

void RandomSampler::generateSamples() {
    int n =sqrt(mSampleNum);
    for(int i=0;i<mSetNum;++i)
        for(int j=0;j<mSampleNum;++j){
            Point v=Point(drand48(),drand48(),0);
            mSamples.push_back(v);
        }
}
