#include <iostream>
#include "MemAllocator.h"

int main()
{
    MemoryAllocator allocator;
    allocator.init();
    int* pFSAsmall1 = (int*)allocator.alloc(sizeof(int));
    int* pFSAsmall2 = (int*)allocator.alloc(sizeof(int));

    int* pFSAbig1 = (int*)allocator.alloc(sizeof(int)*10);
    int* pFSAbig2 = (int*)allocator.alloc(sizeof(int)*10);

    int* p1 = (int*)allocator.alloc(sizeof(int) * 500);
    int* p2 = (int*)allocator.alloc(sizeof(int) * 500);
    int* p3 = (int*)allocator.alloc(sizeof(int) * 500);
    int* p4 = (int*)allocator.alloc(sizeof(int) * 500);
    int* p5 = (int*)allocator.alloc(1904);

    int* p6 = (int*)allocator.alloc(sizeof(int) * 500);
    int* pOS = (int*)allocator.alloc(sizeof(int) * 10485760);

    std::cout << "-----CoalesceAllocTest-----" << std::endl;

    allocator.free(p2);
    allocator.dumpBlocks();

    allocator.free(p4);
    allocator.dumpBlocks();

    allocator.free(p3);
    allocator.dumpBlocks();

    int* p7 = (int*)allocator.alloc(4024);
    allocator.dumpBlocks();

    std::cout << "-----FSAllocTest-----" << std::endl;

    allocator.free(pFSAsmall1);
    allocator.free(pFSAbig2);
    allocator.dumpBlocks();
    allocator.dumpStat();

    std::cout << "-----OSAllocTest-----" << std::endl;

    allocator.free(pOS);

    allocator.dumpBlocks();
    allocator.dumpStat();

    allocator.destroy();
}