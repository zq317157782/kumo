//
// Created by 诸谦 on 16/6/11.
//

#ifndef RAYTRACER_MEMORY_H
#define RAYTRACER_MEMORY_H

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

#endif //RAYTRACER_MEMORY_H
