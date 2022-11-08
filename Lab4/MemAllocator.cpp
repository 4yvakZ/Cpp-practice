#include "MemAllocator.h"


MemoryAllocator::MemoryAllocator() {
#ifdef _DEBUG
	isInitialized = false;
	isDestroyed = false;

	numAlloc = 0;
	numFree = 0;
#endif // _DEBUG
}

MemoryAllocator::~MemoryAllocator() {
#ifdef _DEBUG
	assert(isDestroyed && "MemoryAllocator is not destroyed before delete");
#endif // _DEBUG
}

void MemoryAllocator::init() {
#ifdef _DEBUG
	isInitialized = true;
	assert(!isDestroyed && "MemoryAllocator is not destroyed before init");
	isDestroyed = false;
#endif // _DEBUG
	fsa16.init(16, 512);
	fsa32.init(32, 256);
	fsa64.init(64, 128);
	fsa128.init(128, 64);
	fsa256.init(256, 32);
	fsa512.init(512, 16);
}
void MemoryAllocator::destroy() {
#ifdef _DEBUG
	assert(isInitialized && "MemoryAllocator is not initialized before destroy");
	isDestroyed = true;
	isInitialized = false;
#endif // _DEBUG
	fsa16.destroy();
	fsa32.destroy();
	fsa64.destroy();
	fsa128.destroy();
	fsa256.destroy();
	fsa512.destroy();

	for (size_t i = 0; i < OSBlocks.size(); i++) {
		VirtualFree(OSBlocks[i].data, 0, MEM_RELEASE);
	}
}

void* MemoryAllocator::alloc(size_t size) {
#ifdef _DEBUG
	assert(isInitialized && "MemoryAllocator is not initialized before alloc");
	numAlloc++;
#endif // _DEBUG

	if (size < 16) {
		return fsa16.alloc(size);
	} 
	if (size < 32) {
		return fsa32.alloc(size);
	}
	if (size < 64) {
		return fsa64.alloc(size);
	}
	if (size < 128) {
		return fsa128.alloc(size);
	}
	if (size < 256) {
		return fsa256.alloc(size);
	}
	if (size < 512) {
		return fsa512.alloc(size);
	}

	void* p = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	Block block;
	block.data = p;
	block.size = size;
	OSBlocks.push_back(block);

	return p;
}
void MemoryAllocator::free(void* p) {
#ifdef _DEBUG
	assert(isInitialized && "MemoryAllocator is not initialized before free");
	numFree++;
#endif // _DEBUG
	if (fsa16.free(p)) {
		return;
	}
	if (fsa32.free(p)) {
		return;
	}
	if (fsa64.free(p)) {
		return;
	}
	if (fsa128.free(p)) {
		return;
	}
	if (fsa256.free(p)) {
		return;
	}
	if (fsa512.free(p)) {
		return;
	}

	bool res = VirtualFree(p, 0, MEM_RELEASE);
	assert(res && "Poiner out of bounds");
	if (res) {
		for (auto it = OSBlocks.begin(); it < OSBlocks.end(); it++) {
			if (static_cast<Block>(*it).data == p) {
				OSBlocks.erase(it);
				break;
			}
		}
	}
}

#ifdef _DEBUG
void MemoryAllocator::dumpStat() const {
	std::cout << "Memory Allocator:" << std::endl;
	std::cout << "\tAllocs: " << numAlloc << " Frees: " << numFree << std::endl;

	fsa16.dumpStat();
	fsa32.dumpStat();
	fsa64.dumpStat();
	fsa128.dumpStat();
	fsa256.dumpStat();
	fsa512.dumpStat();

	std::cout << "\tPure OS Blocks:" << std::endl;
	for (size_t i = 0; i < OSBlocks.size(); i++) {
		std::cout << "\t\tBlock " << i << " Adress: " << OSBlocks[i].data << " Size: " << OSBlocks[i].size << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}
void MemoryAllocator::dumpBlocks() const {
	std::cout << "Memory Allocator:" << std::endl;
	fsa16.dumpBlocks();
	fsa32.dumpBlocks();
	fsa64.dumpBlocks();
	fsa128.dumpBlocks();
	fsa256.dumpBlocks();
	fsa512.dumpBlocks(); 
	std::cout << "\tPure OS Blocks:" << std::endl;
	for (size_t i = 0; i < OSBlocks.size(); i++) {
		std::cout << "\t\tBlock " << i << " Adress: " << OSBlocks[i].data << " Size: " << OSBlocks[i].size << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}
#endif