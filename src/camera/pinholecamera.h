//
// Created by 诸谦 on 15/12/28.
//

#ifndef RAYTRACER_PINHOLECAMERA_H
#define RAYTRACER_PINHOLECAMERA_H

#define CAMERA_VIEWPANEL_DISTANCE 200

#include <camera.h>

class PinholeCamera: public Camera{
protected:
    Float mDistanceToView;
    Float mZoomFactor;
    Float xOffset,yOffset;
   // Vector _rayDirection(const Point& p) const;
public:
    PinholeCamera(const Transform& c2w,Film * f,const Float distance=CAMERA_VIEWPANEL_DISTANCE, const Float zoomFactor=1);

    void setDistanceToView(const Float d);


   // virtual void renderScene(const Scene &scene, Film &picture);
    virtual Float GenerateRay(const CameraSample &sample,
                                 Ray *ray) const override;
};


#endif //RAYTRACER_PINHOLECAMERA_H
