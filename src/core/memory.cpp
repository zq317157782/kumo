//
// Created by 诸谦 on 16/6/11.
//

#include "memory.h"
#include "seidennki.h"

void *AllocAligned(unsigned int size) {
	return malloc(size);
}


void FreeAligned(void *ptr) {
    free(ptr);
}
