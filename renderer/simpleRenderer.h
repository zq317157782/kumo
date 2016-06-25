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
    Sampler* sampler;
public:

    SimpleRenderer(Camera* c,Sampler* s):Renderer(),camera(c),sampler(s){
        assert(camera!=nullptr);
        assert(sampler!=nullptr);
    }
    virtual void render(Scene* scene) override;


};


#endif //RAYTRACER_SIMPLERENDERER_H
