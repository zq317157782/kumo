//
// Created by 诸谦 on 16/6/25.
//

#ifndef RAYTRACER_RENDERER_H
#define RAYTRACER_RENDERER_H

#include "global.h"

//渲染器
class Renderer {

public:
    virtual void render(Scene& scene)=0;
};


#endif //RAYTRACER_RENDERER_H
