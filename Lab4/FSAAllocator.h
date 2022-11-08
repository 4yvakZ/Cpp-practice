#pragma once
#include <cassert>
#include "AbstractAllocator.h"

constexpr size_t kNumBlocksInPage = 2;
constexpr int kFreelistEndIndex = -1;

class FSAAllocator : public AbstractAllocator {
public:
	FSAAllocator(size_t blockSize);
	// Inherited via AbstractAllocator
	virtual ~FSAAllocator();

	virtual void init() override;
	virtual void destroy() override;

	virtual void* alloc(size_t size) override;
	virtual void free(void* p) override;

#ifdef _DEBUG
	virtual void dumpStat() const override;
	virtual void dumpBlocks() const override;
#endif

private:
#ifdef _DEBUG
	bool isInitialized = false;
	bool isDestroyed = false;

	size_t numAlloc = 0;
	size_t numFree = 0;
#endif 

	struct Page {
		Page* next;
		size_t FH; ///< Free-List head
		size_t numInitialized;
		void* blocks;
	};
	size_t blockSize;
	Page *page;

	void allocPage(Page*& page);
	void destroyPage(Page*& page);
};