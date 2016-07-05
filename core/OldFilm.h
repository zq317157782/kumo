//
// Created by 诸谦 on 15/12/27.
//

#ifndef RAYTRACER_PICTURE_H
#define RAYTRACER_PICTURE_H

#include "RGB.h"
#include <fstream>
#include "global.h"
/**
 * 照片类  用来存放渲染信息
 */
class OldFilm {
private:
    int mWidth,mHeight;
    float mPixelSize;
    RGB* mData;
public:

    OldFilm(int mWidth, int mHeight, float mPixelSize=1.0);

    virtual ~OldFilm();

    int width() const;
    int height() const;
    float size() const;
    Point resolution() const;
    RGB * const operator[](int k);
    void saveToLocal(const char* file);

};


#endif //RAYTRACER_PICTURE_H
