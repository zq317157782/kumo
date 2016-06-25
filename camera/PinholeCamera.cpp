//
// Created by 诸谦 on 15/12/28.
//

#include "PinholeCamera.h"
PinholeCamera::PinholeCamera(Film * f,const Point & eye, const Point & lookAt, const Vector & up, const float distance, const float zoomFactor)
        :Camera(f,eye,lookAt,up),mDistanceToView(distance),mZoomFactor(zoomFactor){}

void PinholeCamera::setDistanceToView(const float d){
    mDistanceToView=d;
}
Ray PinholeCamera::generateRay(const Point& p) const {
    Vector dir=Normalize(u * p.x + v * p.y + w * mDistanceToView);//注意这里是d*w 构成左手手坐标系统
    Ray ray(eye,dir,0);
    return ray;
}
