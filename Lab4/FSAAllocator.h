#pragma once
#include <cassert>
#include <windows.h>
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

class FSAAllocator {
public:
	FSAAllocator();
	// Inherited via AbstractAllocator
	virtual ~FSAAllocator();

	virtual void init(size_t blockSize, size_t numBlocksInPage);
	virtual void destroy();

	virtual void* alloc(size_t size);
	virtual bool free(void* p);

#ifdef _DEBUG
	virtual void dumpStat() const;
	virtual void dumpBlocks() const;
#endif // _DEBUG

private:
#ifdef _DEBUG
	bool isInitialized;
	bool isDestroyed;

	size_t numAlloc;
	size_t numFree;
#endif  // _DEBUG

	struct Page {
		Page* next;
		size_t FH; ///< Free-List head
		size_t numInitialized;
		void* blocks;
	};
	size_t blockSize;
	size_t numBlocksInPage;
	Page *page;

	void allocPage(Page*& page);
	void destroyPage(Page*& page);
};