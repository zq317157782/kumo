//
// Created by 诸谦 on 15/12/28.
//

#include "Camera.h"

void Camera::computeUVW(){
    w=Normalize(eye - lookAt);
    u=Normalize(Cross(up, w));
    v=Cross(w,u);
}

Camera::Camera(Film* f,const Point & eye, const Point & lookAt, const Vector & up, const float exposureTime):film(f),eye(eye), lookAt(lookAt), up(up), exposureTime(exposureTime), mSampler(NULL){
    computeUVW();
}

void Camera::setSampler(Sampler* sampler) {
    mSampler=sampler;
}
