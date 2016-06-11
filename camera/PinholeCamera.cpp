//
// Created by 诸谦 on 15/12/28.
//

#include "PinholeCamera.h"
Vector PinholeCamera::_rayDirection(const Point& p) const{
    Vector dir=Normalize(u * p.x + v * p.y + w * mDistanceToView);//注意这里是d*w 构成左手手坐标系统
    //std::cout<<"dir:"<<dir.x<<" "<<dir.y<<" "<<dir.z<<std::endl;
    return dir;
}

PinholeCamera::PinholeCamera(const Point & eye, const Point & lookAt, const Vector & up, const float distance, const float zoomFactor)
        :Camera(eye,lookAt,up),mDistanceToView(distance),mZoomFactor(zoomFactor){}

void PinholeCamera::setDistanceToView(const float d){
    mDistanceToView=d;
}

void PinholeCamera::renderScene(const Scene &scene, Film &picture) {

    float pSize=picture.size()/mZoomFactor;//计算缩放后的像素大小
    Ray ray(mEye,Vector(0,0,0),0);//射线;
    RGB L;
    Point point;
    for(int r=0;r<picture.height();++r){
        for(int c=0;c<picture.width();++c){
            float temp=(float)(r*picture.width()+c)/(picture.height()*picture.width())*100;
            L=RGB(0,0,0);
            for(int p=0;p<mSampler->getSampleNum();++p){
                Point v=mSampler->sampleUnitSquare();
                point.x=pSize*(c-picture.width()*0.5+v.x);
                point.y=pSize*(r-picture.height()*0.5+v.y);
                ray.d=_rayDirection(point);
                L+=scene.getTracer()->trace(ray);
            }
            //std::cout<<"Color:"<<L.r<<" "<<L.g<<" "<<L.b<<std::endl;
            RGB color=(L/mSampler->getSampleNum());
            if(color.r>1||color.g>1||color.b>1){
                double max=0;
                if(color.r>max)
                    max=color.r;
                if(color.g>max)
                    max=color.g;
                if(color.b>max)
                    max=color.b;
                picture[r][c]=color/max;
             //   view.frameBuffer[r*view.width()+c]=color/max;
            }
            else
                picture[r][c]=color;
               // view.frameBuffer[r*view.width()+c]=color;
        }
    }
}
