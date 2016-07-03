//
// Created by 诸谦 on 15/12/24.
//

#ifndef RAYTRACER_JITTEREDSAMPLER_H
#define RAYTRACER_JITTEREDSAMPLER_H


#include "../oldSampler/OldSampler.h"

class JitteredSampler: public OldSampler{
public:
    JitteredSampler(int _num_sample=1,int _numSet=DEFAULT_SET_NUM);
    virtual void generateSamples() override;
};


#endif //RAYTRACER_JITTEREDSAMPLER_H
