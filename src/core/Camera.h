//
// Created by 诸谦 on 15/12/28.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H




#include "global.h"

class Camera {
public:
    Film * film;
    Transform *cameraToWorld;
public:
    Camera(Film * f,Transform* c2w);
    virtual float GenerateRay(const CameraSample &sample,
                                 Ray *ray) const = 0;
    virtual float GenerateRayDifferential(const CameraSample &sample, RayDifferential *rd) const;
};


#endif //RAYTRACER_CAMERA_H
