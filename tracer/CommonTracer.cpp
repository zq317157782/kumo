//
// Created by 诸谦 on 15/12/28.
//

#include "CommonTracer.h"
#include "../common/ShadeRec.h"
#include "../scene/Scene.h"
CommonTracer::CommonTracer(Scene&_scene) : Tracer(_scene) { }

RGB CommonTracer::trace(const Ray &ray) const {
    ShadeRec sr(mScene.hit(ray));
    if(sr.hitAnObject){
        sr.ray=ray;
        return sr.material->shade(sr);
    }else{
        return mScene.background;
    }
}
