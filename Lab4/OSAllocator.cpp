#include "OSAllocator.h"

OSAllocator::OSAllocator() {
#ifdef _DEBUG
    isInitialized = false;
    isDestroyed = false;

    numAlloc = 0;
    numFree = 0;
#endif // _DEBUG
}

OSAllocator::~OSAllocator() {
#ifdef _DEBUG
    assert(isDestroyed);
#endif // _DEBUG
}

void OSAllocator::init() {
#ifdef _DEBUG
    isInitialized = true;
    assert(!isDestroyed);
    isDestroyed = false;
#endif // _DEBUG
}
void OSAllocator::destroy() {
#ifdef _DEBUG
    assert(isInitialized);
    isDestroyed = true;
    isInitialized = false;
#endif // _DEBUG
}

void* OSAllocator::alloc(size_t size) {
#ifdef _DEBUG
    assert(isInitialized);
    numAlloc++;
#endif // _DEBUG
	return nullptr;
}
void OSAllocator::free(void* p) {
#ifdef _DEBUG
    assert(isInitialized);
    numFree++;
#endif // _DEBUG

}

#ifdef _DEBUG
void OSAllocator::dumpStat() const {

}
void OSAllocator::dumpBlocks() const {

}
#endif