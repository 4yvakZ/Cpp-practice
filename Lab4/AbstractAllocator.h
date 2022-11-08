#pragma once
#include <windows.h>


class AbstractAllocator {
public:
	virtual ~AbstractAllocator() = 0 {}

	virtual void init() = 0;
	virtual void destroy() = 0;

	virtual void* alloc(size_t size) = 0;
	virtual void free(void* p) = 0;

#ifdef _DEBUG
	virtual void dumpStat() const = 0;
	virtual void dumpBlocks() const = 0;
#endif
};