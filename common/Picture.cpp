//
// Created by 诸谦 on 15/12/27.
//

#include "Picture.h"
Picture::Picture(int mWidth, int mHeight, float mPixelSize) : mWidth(mWidth), mHeight(mHeight), mPixelSize(mPixelSize) {
    mData=new RGB[mWidth*mHeight];
}
Picture::~Picture() {
    delete[] mData;
}

int Picture::width() const{
    return mWidth;
}

int Picture::height() const{
    return mHeight;
}

float Picture::size() const{
    return mPixelSize;
}

Vector2 Picture::resolution() const{
    return Vector2(mWidth,mHeight);
}

RGB * const Picture::operator[](int k)
{
    return mData+(k * mWidth);
}


void Picture::saveToLocal(const char* file){
    std::ofstream out(file);
    out<<"P3\n"<<mWidth<<" "<<mHeight<<"\n255\n";
    for(int j=mHeight-1;j>=0;--j){
        for(int i=0;i<mWidth;++i){
            out<<(int)(mData[j*mWidth+i].r*255)<<" "<<(int)(mData[j*mWidth+i].g*255)<<" "<<(int)(mData[j*mWidth+i].b*255)<<" ";
        }
    }


    // for(int i=0;i<mWidth*mHeight;++i){
    // 	out<<(int)(frameBuffer[i].r*255)<<" "<<(int)(frameBuffer[i].g*255)<<" "<<(int)(frameBuffer[i].b*255)<<" ";
    // }
    out.close();
}

