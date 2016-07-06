//
// Created by 诸谦 on 16/6/25.
//

#include <Camera.h>
#include "simpleRenderer.h"
#include "primitive.h"
#include "integrator.h"
#include "sampler.h"
#include "random.h"
#include "film.h"

void SimpleRenderer::render(const Scene* scene) {
   // float pSize=camera->film->size()/mZoomFactor;//计算缩放后的像素大小
  //  float pSize=camera->film->size();

    Sample sample( sampler ,mSurfaceIntegrator,scene);
    Random ran(0);

    while(sampler->GetMoreSamples(&sample,ran)>0){
    	RGB L;
    	Point point;
    	point.x= sample.imageX-camera->film->xResolution*0.5f;
    	point.y= sample.imageY-camera->film->yResolution*0.5f;
    	Ray ray=camera->generateRay(point);
    	Intersection sr(*scene,ray);
    	if(scene->hit(ray,&sr)){
    	   L=mSurfaceIntegrator->Li(scene,this,ray,sr);
    	}else{
    	   L=scene->background;
    	}
    	camera->film->AddSample(sample,L);
    }

    camera->film->WriteImage(1.0f);

//    for(int r=0;r<camera->film->height();++r){
//        for(int c=0;c<camera->film->width();++c){
//            float temp=(float)(r*camera->film->width()+c)/(camera->film->height()*camera->film->width())*100;
//            L=RGB(0,0,0);
//            for(int p=0;p<sampler->samplesPerPixel;++p){
//                point.x=pSize*(c-camera->film->width()*0.5+sample.lensU);
//                point.y=pSize*(r-camera->film->height()*0.5+sample.lensV);
//                Ray ray=camera->generateRay(point);
//                Intersection sr(*scene,ray);
//                if(scene->hit(ray,&sr)){
//                	L+=mSurfaceIntegrator->Li(scene,this,ray,sr);
//                }else{
//                    L+=scene->background;
//                }
//            }
//            //std::cout<<"Color:"<<L.r<<" "<<L.g<<" "<<L.b<<std::endl;
//            RGB color=(L/sampler->samplesPerPixel);
//            if(color.r>1||color.g>1||color.b>1){
//                double max=0;
//                if(color.r>max)
//                    max=color.r;
//                if(color.g>max)
//                    max=color.g;
//                if(color.b>max)
//                    max=color.b;
//                (*(camera->film))[r][c]=color/max;
//                //   view.frameBuffer[r*view.width()+c]=color/max;
//            }
//            else
//                (*(camera->film))[r][c]=color;
//            // view.frameBuffer[r*view.width()+c]=color;
//        }
//    }
}
