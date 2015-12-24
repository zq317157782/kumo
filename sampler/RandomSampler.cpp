//
// Created by 诸谦 on 15/12/24.
//

#include <math.h>
#include "RandomSampler.h"

RandomSampler::RandomSampler(const int& _sampleNum, const int& _setNum) : Sampler(_sampleNum, _setNum) {
    generateSamples();
}

void RandomSampler::generateSamples() {
    int n =sqrt(mSampleNum);
    for(int i=0;i<mSetNum;++i)
        for(int j=0;j<mSampleNum;++j){
            Vector2 v=Vector2(drand48(),drand48());
            mSamples.push_back(v);
        }
}
