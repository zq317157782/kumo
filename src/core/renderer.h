//
// Created by 诸谦 on 16/6/25.
//

#ifndef RAYTRACER_RENDERER_H
#define RAYTRACER_RENDERER_H

#include <kumo.h>

//渲染器
class Renderer {

public:
    virtual void render(const Scene* scene)=0;
    virtual ~Renderer(){}
    virtual RGB Li(const Scene *scene, const RayDifferential &ray,
            const Sample *sample, Random &rand, MemoryArena &arena,
            Intersection *isect = NULL, RGB *T = NULL) const = 0;
};


#endif //RAYTRACER_RENDERER_H
