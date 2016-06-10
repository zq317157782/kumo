//
// Created by 诸谦 on 15/12/28.
//

#include "Camera.h"

void Camera::computeUVW(){
    w=Normalize(mEye-mLookAt);
    u=Normalize(Cross(mUp,w));
    v=Cross(w,u);
}

Camera::Camera(const Point & eye, const Point & lookAt, const Vector & up, const float exposureTime): mEye(eye), mLookAt(lookAt), mUp(up), mExposureTime(exposureTime), mSampler(NULL){
    computeUVW();
}

void Camera::setSampler(Sampler* sampler) {
    mSampler=sampler;
}
