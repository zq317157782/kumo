//
// Created by 诸谦 on 16/6/25.
//

#include <Camera.h>
#include "simpleRenderer.h"

void SimpleRenderer::render(Scene *scene) {
   // float pSize=camera->film->size()/mZoomFactor;//计算缩放后的像素大小
    float pSize=camera->film->size();
    RGB L;
    Point point;
    for(int r=0;r<camera->film->height();++r){
        for(int c=0;c<camera->film->width();++c){
            float temp=(float)(r*camera->film->width()+c)/(camera->film->height()*camera->film->width())*100;
            L=RGB(0,0,0);
            for(int p=0;p<sampler->getSampleNum();++p){
                Point v=sampler->sampleUnitSquare();
                point.x=pSize*(c-camera->film->width()*0.5+v.x);
                point.y=pSize*(r-camera->film->height()*0.5+v.y);
                Ray ray=camera->generateRay(point);
                ShadeRec sr(scene->hit(ray));
                if(sr.hitAnObject){
                    sr.ray=ray;
                    L+= sr.material->shade(sr);
                }else{
                    L+=scene->background;
                }
            }
            //std::cout<<"Color:"<<L.r<<" "<<L.g<<" "<<L.b<<std::endl;
            RGB color=(L/sampler->getSampleNum());
            if(color.r>1||color.g>1||color.b>1){
                double max=0;
                if(color.r>max)
                    max=color.r;
                if(color.g>max)
                    max=color.g;
                if(color.b>max)
                    max=color.b;
                (*(camera->film))[r][c]=color/max;
                //   view.frameBuffer[r*view.width()+c]=color/max;
            }
            else
                (*(camera->film))[r][c]=color;
            // view.frameBuffer[r*view.width()+c]=color;
        }
    }
}
