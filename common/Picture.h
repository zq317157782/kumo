//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_PICTURE_H
#define RAYTRACER_PICTURE_H

#include "../base/RGB.h"
#include "../base/Vector2.h"
#include <fstream>

/**
 * 照片类  用来存放渲染信息
 */
class Picture {
private:
    int mWidth,mHeight;
    float mPixelSize;
    RGB* mData;
public:

    Picture(int mWidth, int mHeight, float mPixelSize=1.0);

    virtual ~Picture();

    int width() const;
    int height() const;
    float size() const;
    Vector2 resolution() const;
    RGB * const operator[](int k);
    void saveToLocal(const char* file);

};


#endif //RAYTRACER_PICTURE_H
