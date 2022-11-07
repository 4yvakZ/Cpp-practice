#include "MemAlloc.h"

MemoryAllocator::MemoryAllocator() {

}

MemoryAllocator::~MemoryAllocator() {

}

void MemoryAllocator::init() {

}
void MemoryAllocator::destroy() {

}

void* MemoryAllocator::alloc(size_t size) {
	return nullptr;
}
void MemoryAllocator::free(void* p) {

}

#ifdef _DEBUG
void MemoryAllocator::dumpStat() const {

}
void MemoryAllocator::dumpBlocks() const {

}
#endif