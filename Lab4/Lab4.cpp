#include <iostream>
#include "MemAllocator.h"
#include "FSAAllocator.h"

int main()
{
    MemoryAllocator allocator;
    allocator.init();
    int* pa = (int*)allocator.alloc(sizeof(int));
    int* pb = (int*)allocator.alloc(sizeof(int));
    int* pc = (int*)allocator.alloc(sizeof(int));
    int* pd = (int*)allocator.alloc(sizeof(int));
    int* pf = (int*)allocator.alloc(sizeof(int)*1000);

    allocator.free(pf);
    allocator.dumpStat();
    allocator.dumpBlocks();
    allocator.destroy();
}