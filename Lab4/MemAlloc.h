#pragma once
#include <cassert>
#include <windows.h>

class MemoryAllocator {
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
};