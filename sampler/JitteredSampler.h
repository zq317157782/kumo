//
// Created by 诸谦 on 15/12/24.
//

#ifndef RAYTRACER_JITTEREDSAMPLER_H
#define RAYTRACER_JITTEREDSAMPLER_H


#include "Sampler.h"

class JitteredSampler: public Sampler{
public:
    JitteredSampler(const int& _num_sample=1,const int& _numSet=DEFAULT_SET_NUM);
    virtual void generateSamples() override;
};


#endif //RAYTRACER_JITTEREDSAMPLER_H
