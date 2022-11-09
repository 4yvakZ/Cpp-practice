#pragma once
#include <cassert>
#include <windows.h>

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

class CoalesceAllocator {
public:
	CoalesceAllocator();

	virtual ~CoalesceAllocator();

	virtual void init(size_t OSBlockSize);
	virtual void destroy();

	virtual void* alloc(size_t size);
	virtual bool free(void* p);

#ifdef _DEBUG
	virtual void dumpStat() const;
	virtual void dumpBlocks() const;
#endif

private:
	struct Buffer {
		Buffer* next;
		size_t FH;
		void* blocks;
	};
	struct Block {
		size_t size;
		void* data;
		bool free;
	};
#ifdef _DEBUG
	bool isInitialized;
	bool isDestroyed;

	size_t numAlloc;
	size_t numFree;
#endif  // _DEBUG
	size_t bufferSize;
	Buffer* buffer;

	void allocBuffer(Buffer*& buffer);
	void destroyBuffer(Buffer*& buffer);
};