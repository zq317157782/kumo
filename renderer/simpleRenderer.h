//
// Created by 诸谦 on 16/6/25.
//

#ifndef RAYTRACER_SIMPLERENDERER_H
#define RAYTRACER_SIMPLERENDERER_H
#include "global.h"
#include "renderer.h"

class SimpleRenderer :public Renderer{
private:
    Camera* camera;
    OldSampler* sampler;
    SurfaceIntegrator* mSurfaceIntegrator;
public:

    SimpleRenderer(Camera* c,OldSampler* s,SurfaceIntegrator* si):Renderer(),camera(c),sampler(s),mSurfaceIntegrator(si){
        assert(camera!=nullptr);
        assert(sampler!=nullptr);
    }
    virtual void render(const Scene* scene) override;


};


#endif //RAYTRACER_SIMPLERENDERER_H
