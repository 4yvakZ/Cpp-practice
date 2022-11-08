#include <iostream>
#include "MemAllocator.h"
#include "FSAAllocator.h"

int main()
{
    FSAAllocator allocator(64);
    allocator.init();
    int* pa = (int*)allocator.alloc(sizeof(int));
    int* pb = (int*)allocator.alloc(sizeof(int));
    int* pc = (int*)allocator.alloc(sizeof(int));
    int* pd = (int*)allocator.alloc(sizeof(int));
    allocator.free(pb);
    allocator.dumpStat();
    allocator.dumpBlocks();
    allocator.destroy();
}