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

   // Vector _rayDirection(const Point& p) const;
public:
    PinholeCamera(Film * f,Transform* c2w,const float distance=CAMERA_VIEWPANEL_DISTANCE, const float zoomFactor=1);

    void setDistanceToView(const float d);


   // virtual void renderScene(const Scene &scene, Film &picture);
   virtual Ray generateRay(const Point& p) const override;
};


#endif //RAYTRACER_PINHOLECAMERA_H
