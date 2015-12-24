//
// Created by 诸谦 on 15/12/24.
//

#ifndef RAYTRACER_NROOKSAMPLER_H
#define RAYTRACER_NROOKSAMPLER_H


#include "Sampler.h"

class NRookSampler : public Sampler{
public:
    NRookSampler(const int& _sampleNum=1, const int& _setNum=DEFAULT_SET_NUM);
    virtual void generateSamples() override;
private:
    void _shuffleXCoordinates();
    void _shuffleYCoordinates();
};


#endif //RAYTRACER_NROOKSAMPLER_H
