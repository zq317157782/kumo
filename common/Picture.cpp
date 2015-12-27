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


