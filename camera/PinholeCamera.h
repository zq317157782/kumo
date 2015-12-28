//
// Created by 诸谦 on 15/12/28.
//

#ifndef RAYTRACER_PINHOLECAMERA_H
#define RAYTRACER_PINHOLECAMERA_H
#define CAMERA_VIEWPANEL_DISTANCE 200

#include "Camera.h"

class PinholeCamera: public Camera{
protected:
    float mDistanceToView;
    float mZoomFactor;

    Vector3 _rayDirection(const Vector2& p) const;
public:
    PinholeCamera(const Vector3& eye=Vector3(0,0,0),const Vector3& lookAt=Vector3(0,0,-1),const Vector3& up=Vector3(0,1,0),const float distance=CAMERA_VIEWPANEL_DISTANCE,const float zoomFactor=1);

    void setDistanceToView(const float d);


    virtual void renderScene(const Scene &scene,Picture &picture);
};


#endif //RAYTRACER_PINHOLECAMERA_H
