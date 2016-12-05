/*
 * texture.cpp
 *
 *  Created on: 2016年8月24日
 *      Author: zhuqian
 */
#include "texture.h"

//TODO Lanczos相关的过滤器中使用的函数，现在先不去深究它的逻辑。
Float Lanczos(Float x, Float tau) {
    x = fabsf(x);
    if (x < 1e-5) return 1;
    if (x > 1.)    return 0;
    x *= Pi;
    Float s = sinf(x * tau) / (x * tau);
    Float lanczos = sinf(x) / x;
    return s * lanczos;
}



