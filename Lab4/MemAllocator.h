#pragma once
#include <cassert>
#include <windows.h>

#include "FSAAllocator.h"

#ifdef _DEBUG
#include <iostream>
#include <vector>
#endif // _DEBUG

class MemoryAllocator{
public:
	MemoryAllocator();

	virtual ~MemoryAllocator();

	virtual void init();
	virtual void destroy();

	virtual void *alloc(size_t size);
	virtual void free(void* p);

#ifdef _DEBUG
	virtual void dumpStat() const;
	virtual void dumpBlocks() const;
#endif

private:
	
#ifdef _DEBUG
	bool isInitialized;
	bool isDestroyed;

	size_t numAlloc;
	size_t numFree;
#endif  // _DEBUG
	struct Block {
		size_t size;
		void* data;
	};
	std::vector<Block> OSBlocks;
	FSAAllocator fsa16, fsa32, fsa64, fsa128, fsa256, fsa512;
};