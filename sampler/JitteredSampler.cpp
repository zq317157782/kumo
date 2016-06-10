//
// Created by 诸谦 on 15/12/24.
//

#include <math.h>
#include "JitteredSampler.h"

JitteredSampler::JitteredSampler(int _num_sample,int _numSet):Sampler(_num_sample,_numSet){
    generateSamples();
}

void JitteredSampler::generateSamples() {
    int n =sqrt(mSampleNum);
    for(int i=0;i<mSetNum;++i)
        for(int j=0;j<n;++j)
            for(int k=0;k<n;++k){
                Point v=Point((k+drand48())/n,(j+drand48())/n,0);
                mSamples.push_back(v);
            }
}
