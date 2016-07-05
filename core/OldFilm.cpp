//
// Created by 诸谦 on 15/12/27.
//

#include <OldFilm.h>
#include "global.h"
#include "geometry.h"
OldFilm::OldFilm(int mWidth, int mHeight, float mPixelSize) : mWidth(mWidth), mHeight(mHeight), mPixelSize(mPixelSize) {
    mData=new RGB[mWidth*mHeight];
}
OldFilm::~OldFilm() {
    delete[] mData;
}

int OldFilm::width() const{
    return mWidth;
}

int OldFilm::height() const{
    return mHeight;
}

float OldFilm::size() const{
    return mPixelSize;
}

Point OldFilm::resolution() const{
    return Point(mWidth,mHeight,0);
}

RGB * const OldFilm::operator[](int k)
{
    return mData+(k * mWidth);
}


void OldFilm::saveToLocal(const char* file){
    std::ofstream out(file);
    out<<"P3\n"<<mWidth<<" "<<mHeight<<"\n255\n";
    for(int j=mHeight-1;j>=0;--j){
        for(int i=0;i<mWidth;++i){
            RGB finalColor=mData[j*mWidth+i].clamp();
            out<<(int)(finalColor.r*255)<<" "<<(int)(finalColor.g*255)<<" "<<(int)(finalColor.b*255)<<" ";
        }
    }


    // for(int i=0;i<mWidth*mHeight;++i){
    // 	out<<(int)(frameBuffer[i].r*255)<<" "<<(int)(frameBuffer[i].g*255)<<" "<<(int)(frameBuffer[i].b*255)<<" ";
    // }
    out.close();
}

