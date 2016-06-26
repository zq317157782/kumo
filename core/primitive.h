//
// Created by 诸谦 on 16/6/11.
//

#ifndef RAYTRACER_PRIMITIVE_H
#define RAYTRACER_PRIMITIVE_H
#include "global.h"
#include "memory.h"
#include "shape.h"
#include "Material.h"
//图元
class Primitive:public ReferenceCounted{
protected:
    static unsigned int nextPrimitiveID;
public:
    const unsigned int primitiveID;
    Primitive():primitiveID(nextPrimitiveID++){}
};


#endif //RAYTRACER_PRIMITIVE_H
