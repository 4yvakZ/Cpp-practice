#include "CoalesceAllocator.h"

constexpr int kFreelistEndIndex = -1;

CoalesceAllocator::CoalesceAllocator() {
#ifdef _DEBUG
    isInitialized = false;
    isDestroyed = false;

    numAlloc = 0;
    numFree = 0;
#endif // _DEBUG
}

CoalesceAllocator::~CoalesceAllocator() {
#ifdef _DEBUG
    assert(isDestroyed && "CoalesceAllocator is not destroyed before delete");
#endif // _DEBUG
}

void CoalesceAllocator::init(size_t OSBlockSize) {
#ifdef _DEBUG
    isInitialized = true;
    assert(!isDestroyed && "CoalesceAllocator is not destroyed before init");
    isDestroyed = false;
#endif // _DEBUG
    this->OSBlockSize = OSBlockSize;
}
void CoalesceAllocator::destroy() {
#ifdef _DEBUG
    assert(isInitialized && "CoalesceAllocator is not initialized before destroy");
    isDestroyed = true;
    isInitialized = false;
#endif // _DEBUG
}

void* CoalesceAllocator::alloc(size_t size) {
#ifdef _DEBUG
    assert(isInitialized && "CoalesceAllocator is not initialized before alloc");
    numAlloc++;
#endif // _DEBUG

    return nullptr;
}
bool CoalesceAllocator::free(void* p) {
#ifdef _DEBUG
    assert(isInitialized && "CoalesceAllocator is not initialized before free");
    numFree++;
#endif // _DEBUG

    return false;
}

#ifdef _DEBUG
void CoalesceAllocator::dumpStat() const {
    assert(isInitialized && "CoalesceAllocator is not initialized before dumpStat");

}
void CoalesceAllocator::dumpBlocks() const {
    assert(isInitialized && "CoalesceAllocator is not initialized before dumpBlocks");

}
#endif