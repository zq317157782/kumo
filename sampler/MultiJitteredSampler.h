//
// Created by 诸谦 on 15/12/24.
//

#ifndef RAYTRACER_MULTIJITTEREDSAMPLER_H
#define RAYTRACER_MULTIJITTEREDSAMPLER_H


#include "Sampler.h"

class MultiJitteredSampler: public Sampler{

public:
    MultiJitteredSampler(const int& _sampleNum=1, const int& _setNum=DEFAULT_SET_NUM);

    virtual void generateSamples() override;
};


#endif //RAYTRACER_MULTIJITTEREDSAMPLER_H
