#pragma once
#include <cassert>

#include "AbstractAllocator.h"

class MemoryAllocator: public AbstractAllocator{
public:
	MemoryAllocator();

	virtual ~MemoryAllocator();

	virtual void init() override;
	virtual void destroy() override;

	virtual void *alloc(size_t size) override;
	virtual void free(void* p) override;

#ifdef _DEBUG
	virtual void dumpStat() const override;
	virtual void dumpBlocks() const override;
#endif
};