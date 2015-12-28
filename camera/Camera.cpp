//
// Created by 诸谦 on 15/12/28.
//

#include "Camera.h"

void Camera::computeUVW(){
    w=mEye-mLookAt;
    w.normalize();
    u=mUp.cross(w);
    u.normalize();
    v=w.cross(u);
}

Camera::Camera(const Vector3& eye,const Vector3& lookAt,const Vector3& up,const float exposureTime):mEye(eye),mLookAt(lookAt),mUp(up),mExposureTime(exposureTime),mSampler(NULL){
    computeUVW();
}

void Camera::setSampler(Sampler* sampler) {
    mSampler=sampler;
}
