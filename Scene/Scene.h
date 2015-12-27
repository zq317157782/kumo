//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H
#include <memory>
#include <bits/stl_bvector.h>
#include "../light/Ambient.h"

using namespace std;
class Scene {
private:
    shared_ptr<Light> mAmbient;//环境光
    vector< shared_ptr<Light>> mLights;
public:

    Scene(const shared_ptr<Light> &mAmbient=shared_ptr<Light>(new Ambient()));
    unsigned long getLightNum() const;
    const shared_ptr<Light> ambient() const;
    const  shared_ptr<Light> getLight(const int index) const;
};


#endif //RAYTRACER_SCENE_H
