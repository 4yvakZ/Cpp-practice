#include "FSAAllocator.h"
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

FSAAllocator::FSAAllocator() {
#ifdef _DEBUG
    isInitialized = false;
    isDestroyed = false;

    numAlloc = 0;
    numFree = 0;
#endif // _DEBUG
}

FSAAllocator::~FSAAllocator() {
#ifdef _DEBUG
    assert(isDestroyed && "FSA is not destroyed before delete");
#endif // _DEBUG
}

void FSAAllocator::init(size_t blockSize,size_t numBlocksInPage) {
#ifdef _DEBUG
    isInitialized = true;
    assert(!isDestroyed && "FSA is not destroyed before init");
    isDestroyed = false;
#endif // _DEBUG
    this->blockSize = blockSize;
    this->numBlocksInPage = numBlocksInPage;
    allocPage(page);
}

void FSAAllocator::destroy() {
#ifdef _DEBUG
    assert(isInitialized && "FSA is not initialized before destroy");
    isDestroyed = true;
    isInitialized = false;
#endif // _DEBUG
    destroyPage(page);
    page = nullptr;
}

void* FSAAllocator::alloc(size_t size) {
#ifdef _DEBUG
    assert(isInitialized && "FSA is not initialized before alloc");
    numAlloc++;
#endif // _DEBUG

    Page* currentPage = page;

    while (currentPage->FH == kFreelistEndIndex) {
        if (currentPage->numInitialized < numBlocksInPage) {
            currentPage->numInitialized++;
            void* p = static_cast<char*>(currentPage->blocks) + (currentPage->numInitialized - 1) * blockSize;
            return p;
        }
        if (currentPage->next == nullptr) {
            allocPage(currentPage->next);
        }
        currentPage = currentPage->next;
    }
    void* p = static_cast<char*>(currentPage->blocks) + currentPage->FH * blockSize;
    currentPage->FH = *static_cast<size_t*>(p);
    return p;
}

bool FSAAllocator::free(void* p) {
#ifdef _DEBUG
    assert(isInitialized && "FSA is not initialized before free");
    numFree++;
#endif // _DEBUG
    Page* currentPage = page;
    while (currentPage != nullptr) {
        if (currentPage->blocks < p && (char*)currentPage->blocks + numBlocksInPage * blockSize > p) {
            *static_cast<size_t*>(p) = currentPage->FH;
            currentPage->FH = static_cast<size_t>((static_cast<char*>(p) - static_cast<char*>(currentPage->blocks))/blockSize);
            return true;
        }
        currentPage = currentPage->next;
    }
    return false;
}

#ifdef _DEBUG
void FSAAllocator::dumpStat() const {
    assert(isInitialized && "FSA is not initialized before dumpStat");
    std::cout << "\tFSA " << blockSize << ":" << std::endl;
    std::cout << "\t\tAllocs: " << numAlloc << " Frees: " << numFree << std::endl;

    Page* currentPage = page;
    size_t numPages = 0;
    size_t numBusyBlocks = 0;
    size_t numFreeBlocks = 0;
    while (currentPage != nullptr) {
        for (size_t i = 0; i < currentPage->numInitialized; i++) {
            bool isFree = false;
            size_t index = currentPage->FH;
            while (index != kFreelistEndIndex) {
                if (i == index) {
                    isFree = true;
                    break;
                }
                index = *static_cast<size_t*>(static_cast<void*>(static_cast<char*>(currentPage->blocks) + index * blockSize));
            }
            if (!isFree) {
                numBusyBlocks++;
            }
            else {
                numFreeBlocks++;
            }
        }
        currentPage = currentPage->next;
        numPages++;
    }

    std::cout << "\t\tBusy Blocks: " << numBusyBlocks << " Free Blocks: " << numFreeBlocks << std::endl;
    std::cout << "\t\tOS Blocks:" << std::endl;

    currentPage = page;
    size_t pageIndex = 0;
    while (currentPage != nullptr) {
        std::cout << "\t\t\tBlock " << pageIndex << " Adress: " << static_cast<void*>(currentPage) << " Size: " << blockSize * numBlocksInPage + sizeof(Page) << std::endl;
        pageIndex++;
        currentPage = currentPage->next;
    }
}

void FSAAllocator::dumpBlocks() const {
    assert(isInitialized && "FSA is not initialized before dumpBlocks");
    std::cout << "\tFSA " << blockSize << ":" << std::endl;
    Page* currentPage = page;
    size_t pageIndex = 0;
    while (currentPage != nullptr) {
        for (size_t i = 0; i < currentPage->numInitialized; i++) {
            bool isFree = false; 
            size_t index = currentPage->FH;
            while (index != kFreelistEndIndex) {
                if (i == index) {
                    isFree = true;
                    break;
                }
                index = *static_cast<size_t*>(static_cast<void*>(static_cast<char*>(currentPage->blocks) + index * blockSize));
            }
            std::cout << "\t\tPage " << pageIndex << " Block " << i;
            if (!isFree) {
                std::cout << " Busy";
            }
            else {
                std::cout << " Free";
            }

            std::cout<< " Adress: " << static_cast<void*>(static_cast<char*>(currentPage->blocks) + i * blockSize)<< std::endl;
        }
        currentPage = currentPage->next;
        pageIndex++;
    }
}
#endif // _DEBUG

void FSAAllocator::allocPage(Page*& page) {
    void* pBuf = VirtualAlloc(NULL, blockSize * numBlocksInPage + sizeof(Page), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    page = static_cast<Page*>(pBuf);
    page->next = nullptr;
    page->FH = kFreelistEndIndex;
    page->blocks = static_cast<char*>(pBuf) + sizeof(Page);
    page->numInitialized = 0;
}

void FSAAllocator::destroyPage(Page*& page) {
    if (page == nullptr) {
        return;
    }
    destroyPage(page->next);
    VirtualFree(static_cast<void*>(page), 0, MEM_RELEASE);
}
