//
// Created by 诸谦 on 15/12/24.
//

#ifndef RAYTRACER_RANDOMSAMPLER_H
#define RAYTRACER_RANDOMSAMPLER_H


#include "Sampler.h"

class RandomSampler: public Sampler{

public:
    RandomSampler( int _sampleNum=1, int _setNum=DEFAULT_SET_NUM);
    virtual void generateSamples() override;
};


#endif //RAYTRACER_RANDOMSAMPLER_H
