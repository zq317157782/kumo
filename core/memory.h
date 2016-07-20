//
// Created by 诸谦 on 16/6/11.
//

#ifndef RAYTRACER_MEMORY_H
#define RAYTRACER_MEMORY_H

#include "global.h"
// Memory Declarations
class ReferenceCounted {
public:
    ReferenceCounted() { nReferences = 0; }
    int nReferences;
private:
    ReferenceCounted(const ReferenceCounted &);
    ReferenceCounted &operator=(const ReferenceCounted &);
};


template <typename T> class Reference {
public:
    // Reference Public Methods
    Reference(T *p = nullptr) {
        ptr = p;
        if (ptr) ptr->nReferences+=1;
    }
    Reference(const Reference<T> &r) {
        ptr = r.ptr;
        if (ptr)ptr->nReferences+=1;
    }
    Reference &operator=(const Reference<T> &r) {
        if (r.ptr) r.ptr->nReferences+=1;
        if (ptr && (ptr->nReferences-=1) == 0) delete ptr;
        ptr = r.ptr;
        return *this;
    }
    Reference &operator=(T *p) {
        if (p) p->nReferences+=1;
        if (ptr && (ptr->nReferences-=1) == 0) delete ptr;
        ptr = p;
        return *this;
    }
    ~Reference() {
        if (ptr &&(ptr->nReferences-=1) == 0)
            delete ptr;
    }
    T *operator->() { return ptr; }
    const T *operator->() const { return ptr; }
    operator bool() const { return ptr != nullptr; }
    const T *GetPtr() const { return ptr; }
private:
    T *ptr;
};

//分配内存的方法 现在只实现非对齐方式分配
void *AllocAligned(unsigned int  size);
template <typename T> T *AllocAligned(unsigned int count) {
    return (T *)AllocAligned(count * sizeof(T));
}
void FreeAligned(void *);

//内存分配管理结构  释放内存只能使用FreeAll释放所有的内存
class MemoryArena {
public:
    // MemoryArena Public Methods
    MemoryArena(unsigned int bs = 32768) {
        blockSize = bs;
        curBlockPos = 0;
        currentBlock = AllocAligned<char>(blockSize);
    }
    ~MemoryArena() {
        FreeAligned(currentBlock);
        for (unsigned int i = 0; i < usedBlocks.size(); ++i)
            FreeAligned(usedBlocks[i]);
        for (unsigned int i = 0; i < availableBlocks.size(); ++i)
            FreeAligned(availableBlocks[i]);
    }
    void *Alloc(unsigned int sz) {
        // Round up _sz_ to minimum machine alignment
        sz = ((sz + 15) & (~15));
        if (curBlockPos + sz > blockSize) {
            // Get new block of memory for _MemoryArena_
            usedBlocks.push_back(currentBlock);
            if (availableBlocks.size() && sz <= blockSize) {
                currentBlock = availableBlocks.back();
                availableBlocks.pop_back();
            }
            else
                currentBlock = AllocAligned<char>(max(sz, blockSize));
            curBlockPos = 0;
        }
        void *ret = currentBlock + curBlockPos;
        curBlockPos += sz;
        return ret;
    }
    template<typename T> T *Alloc(unsigned int count = 1) {
        T *ret = (T *)Alloc(count * sizeof(T));
        for (unsigned int i = 0; i < count; ++i)
            new (&ret[i]) T();
        return ret;
    }
    void FreeAll() {
        curBlockPos = 0;
        while (usedBlocks.size()) {
    #ifndef NDEBUG
            memset(usedBlocks.back(), 0xfa, blockSize);
    #endif
            availableBlocks.push_back(usedBlocks.back());
            usedBlocks.pop_back();
        }
    }
private:
    // MemoryArena Private Data
    unsigned int curBlockPos, blockSize;
    char *currentBlock;
    vector<char *> usedBlocks, availableBlocks;
};


#endif //RAYTRACER_MEMORY_H
