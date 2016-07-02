/*
 * sampler.cpp
 *
 *  Created on: 2016年7月1日
 *      Author: Administrator
 */

#include <sampler.h>
#include "global.h"

void Sampler::ComputeSubWindow(int num, int count, int *newXStart,
        int *newXEnd, int *newYStart, int *newYEnd) const {
    // Determine how many tiles to use in each dimension, _nx_ and _ny_
    int dx = xPixelEnd - xPixelStart, dy = yPixelEnd - yPixelStart;
    int nx = count, ny = 1;
    while ((nx & 0x1) == 0 && 2 * dx * ny < dy * nx) {
        nx >>= 1;
        ny <<= 1;
    }
    assert(nx * ny == count);

    // Compute $x$ and $y$ pixel sample range for sub-window
    int xo = num % nx, yo = num / nx;
    float tx0 = float(xo) / float(nx), tx1 = float(xo+1) / float(nx);
    float ty0 = float(yo) / float(ny), ty1 = float(yo+1) / float(ny);
    *newXStart = Floor2Int(Lerp(tx0, xPixelStart, xPixelEnd));
    *newXEnd   = Floor2Int(Lerp(tx1, xPixelStart, xPixelEnd));
    *newYStart = Floor2Int(Lerp(ty0, yPixelStart, yPixelEnd));
    *newYEnd   = Floor2Int(Lerp(ty1, yPixelStart, yPixelEnd));
}
