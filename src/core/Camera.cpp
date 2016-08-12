//
// Created by 诸谦 on 15/12/28.
//

#include "Camera.h"
#include "film.h"

Camera::Camera(Film * f,Transform *c2w):film(f),cameraToWorld(c2w){

}
