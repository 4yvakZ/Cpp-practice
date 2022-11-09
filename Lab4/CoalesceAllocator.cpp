#include "CoalesceAllocator.h"

constexpr int kFreelistEndIndex = -1;

CoalesceAllocator::CoalesceAllocator() {
#ifdef _DEBUG
    isInitialized = false;
    isDestroyed = false;

    numAlloc = 0;
    numFree = 0;
#endif // _DEBUG
}

CoalesceAllocator::~CoalesceAllocator() {
#ifdef _DEBUG
    assert(isDestroyed && "CoalesceAllocator is not destroyed before delete");
#endif // _DEBUG
}

void CoalesceAllocator::init(size_t OSBlockSize) {
#ifdef _DEBUG
    isInitialized = true;
    assert(!isDestroyed && "CoalesceAllocator is not destroyed before init");
    isDestroyed = false;
#endif // _DEBUG
    this->bufferSize = OSBlockSize;
    allocBuffer(buffer);
}

void CoalesceAllocator::destroy() {
#ifdef _DEBUG
    assert(isInitialized && "CoalesceAllocator is not initialized before destroy");
    isDestroyed = true;
    isInitialized = false;
#endif // _DEBUG
    destroyBuffer(buffer);
}

void* CoalesceAllocator::alloc(size_t size) {
#ifdef _DEBUG
    assert(isInitialized && "CoalesceAllocator is not initialized before alloc");
    numAlloc++;
#endif // _DEBUG
    Buffer* currentBuffer = buffer;
    while (true) {
        if (currentBuffer->FH != kFreelistEndIndex) {
            Block* currentBlock = static_cast<Block*>(static_cast<void*>(static_cast<char*>(currentBuffer->blocks) + currentBuffer->FH));
            Block* prevBlock = nullptr;
            size_t freelistIndex;
            while (true) {
                freelistIndex = *static_cast<size_t*>(currentBlock->data);
                if (currentBlock->size >= size) {

                    if (currentBlock->size == size) {
                        if (prevBlock == nullptr) {
                            currentBuffer->FH = freelistIndex;
                        }
                        else {
                            *static_cast<size_t*>(prevBlock->data) = kFreelistEndIndex;
                        }
                        currentBlock->free = false;
                        return currentBlock->data;
                    }

                    size_t oldSize = currentBlock->size;
                    currentBlock->size = size;
                    Block* newBlock = static_cast<Block*>(static_cast<void*>(static_cast<char*>(currentBlock->data) + size));
                    newBlock->data = static_cast<char*>(static_cast<void*>(newBlock)) + sizeof(Block);
                    newBlock->size = oldSize - size - sizeof(Block);
                    newBlock->free = true;
                    *static_cast<size_t*>(newBlock->data) = kFreelistEndIndex;
                    if (prevBlock == nullptr) {
                        currentBuffer->FH = static_cast<char*>(static_cast<void*>(newBlock)) - static_cast<char*>(currentBuffer->blocks);
                    }
                    else {
                        *static_cast<size_t*>(prevBlock->data) = static_cast<char*>(static_cast<void*>(newBlock)) - static_cast<char*>(currentBuffer->blocks);
                    }
                    currentBlock->free = false;
                    return currentBlock->data;
                }

                if (freelistIndex == kFreelistEndIndex) {
                    break;
                }

                prevBlock = currentBlock;
                currentBlock = static_cast<Block*>(static_cast<void*>(static_cast<char*>(currentBuffer->blocks) + freelistIndex));
            }
        }
    
        if (currentBuffer->next == nullptr) {
            allocBuffer(currentBuffer->next);
        }
        currentBuffer = currentBuffer->next;
    }
}


bool CoalesceAllocator::free(void* p) {
#ifdef _DEBUG
    assert(isInitialized && "CoalesceAllocator is not initialized before free");
    numFree++;
#endif // _DEBUG
    Buffer* currentBuffer = buffer;
    while (currentBuffer != nullptr) {
        if (currentBuffer->blocks < p && (char*)currentBuffer->blocks + bufferSize + sizeof(Block) > p) {
            Block* currentBlock = static_cast<Block*>(static_cast<void*>(static_cast<char*>(currentBuffer->blocks)));
            Block* prevBlock = nullptr;
            while (currentBlock->data < p) {
                prevBlock = currentBlock;
                currentBlock = static_cast<Block*>(static_cast<void*>(static_cast<char*>(currentBlock->data) + currentBlock->size));
            }

            Block* nextBlock = static_cast<Block*>(static_cast<void*>(static_cast<char*>(currentBlock->data) + currentBlock->size));

            if (static_cast<void*>(nextBlock) >= (char*)currentBuffer->blocks + bufferSize + sizeof(Block)) {
                nextBlock = nullptr;
            }

            if (nextBlock != nullptr && nextBlock->free) {
                size_t nextBlockFLIndex = currentBuffer->FH;
                size_t prevFLIndex = kFreelistEndIndex;
                while (static_cast<Block*>(static_cast<void*>(static_cast<char*>(currentBuffer->blocks) + nextBlockFLIndex)) != nextBlock) {
                    prevFLIndex = nextBlockFLIndex;
                    nextBlockFLIndex = *static_cast<size_t*>(static_cast<Block*>(static_cast<void*>(static_cast<char*>(currentBuffer->blocks) + nextBlockFLIndex))->data);
                }

                // ѕеренапр€вл€ем указание на nextBlock на указание из nextBlock
                if (prevFLIndex == kFreelistEndIndex) { //Ќа блок с указанием на nextBlock указывает голова Freelist
                    currentBuffer->FH = *static_cast<size_t*>(nextBlock->data);
                }
                else {//Ќа блок с указанием на nextBlock указывает блок с индексом prevFLIndex
                    *static_cast<size_t*>(static_cast<Block*>(static_cast<void*>(static_cast<char*>(currentBuffer->blocks) + prevFLIndex))->data) =
                        *static_cast<size_t*>(nextBlock->data);;
                }

                //—ли€ние nextBlock в currentBlock
                currentBlock->size += nextBlock->size + sizeof(Block);
            }

            if (prevBlock != nullptr && prevBlock->free) {
                //—ли€ние currentBlock в prevBlock (указатели не мен€ем, так как на prevBlock уже должен быть указатель)
                prevBlock->size += currentBlock->size + sizeof(Block);
                return true;
            }

            currentBlock->free = true;
            *static_cast<size_t*>(currentBlock->data) = currentBuffer->FH;
            currentBuffer->FH = static_cast<char*>(static_cast<void*>(currentBlock)) - static_cast<char*>(currentBuffer->blocks);
            
            return true;
        }
        currentBuffer = currentBuffer->next;
    }

#ifdef _DEBUG
    numFree--;
#endif // _DEBUG
    return false;
}

#ifdef _DEBUG
void CoalesceAllocator::dumpStat() const {
    assert(isInitialized && "CoalesceAllocator is not initialized before dumpStat");

    std::cout << "\tCoalesceAllocator:" << std::endl;
    std::cout << "\t\tAllocs: " << numAlloc << " Frees: " << numFree << std::endl;

    Buffer* currentBuffer = buffer;
    size_t numBusyBlocks = 0;
    size_t numFreeBlocks = 0;

    while (currentBuffer != nullptr) {
        Block* currentBlock = static_cast<Block*>(currentBuffer->blocks);
        while (static_cast<char*>(static_cast<void*>(currentBlock)) - static_cast<char*>(currentBuffer->blocks) < bufferSize + sizeof(Block)) {
            
            if (currentBlock->free) {
                numFreeBlocks++;
            }
            else {
                numBusyBlocks++;
            }

            currentBlock = static_cast<Block*>(static_cast<void*>(static_cast<char*>(currentBlock->data) + currentBlock->size));
        }
        currentBuffer = currentBuffer->next;
    }

    std::cout << "\t\tBusy Blocks: " << numBusyBlocks << " Free Blocks: " << numFreeBlocks << std::endl;

    std::cout << "\t\tOS Buffers:" << std::endl;
    currentBuffer = buffer;
    size_t bufferIndex = 0;
    while (currentBuffer != nullptr) {
        std::cout << "\t\t\tBuffer " << bufferIndex << " Adress: " << static_cast<void*>(currentBuffer) 
            << " Size: " << bufferSize + sizeof(Buffer) + sizeof(Block) << std::endl;
        bufferIndex++;
        currentBuffer = currentBuffer->next;
    }
}


void CoalesceAllocator::dumpBlocks() const {
    assert(isInitialized && "CoalesceAllocator is not initialized before dumpBlocks");

    std::cout << "\tCoalesceAllocator:" << std::endl;

    Buffer* currentBuffer = buffer;
    size_t bufferIndex = 0;

    while (currentBuffer != nullptr) {
        std::cout << "\t\tBuffer " << bufferIndex << std::endl;
        Block* currentBlock = static_cast<Block*>(currentBuffer->blocks);
        size_t blockIndex = 0;
        while (static_cast<char*>(static_cast<void*>(currentBlock)) - static_cast<char*>(currentBuffer->blocks) < bufferSize + sizeof(Block)) {

            std::cout << "\t\t\tBlock " << blockIndex;
            if (currentBlock->free) {
                std::cout << " Free";
            }
            else {
                std::cout << " Busy";
            }

            blockIndex++;

            std::cout << " Adress: " << static_cast<void*>(currentBlock) << " Size " << currentBlock->size << std::endl;

            currentBlock = static_cast<Block*>(static_cast<void*>(static_cast<char*>(currentBlock->data) + currentBlock->size));
        }
        currentBuffer = currentBuffer->next;
        bufferIndex++;
    }
}
#endif

void CoalesceAllocator::allocBuffer(Buffer*& buffer)
{
    void* pBuf = VirtualAlloc(NULL, bufferSize + sizeof(Buffer) + sizeof(Block), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    buffer = static_cast<Buffer*>(pBuf);
    buffer->next = nullptr;
    buffer->FH = 0;
    buffer->blocks = static_cast<char*>(pBuf) + sizeof(Buffer);
    Block* b = static_cast<Block*>(buffer->blocks);
    b->size = bufferSize;
    b->data = static_cast<char*>(static_cast<void*>(b)) + sizeof(Block);
    b->free = true;
    *static_cast<size_t*>(b->data) = kFreelistEndIndex;
}

void CoalesceAllocator::destroyBuffer(Buffer*& buffer)
{
    if (buffer == nullptr) {
        return;
    }
    destroyBuffer(buffer->next);
    VirtualFree(static_cast<void*>(buffer), 0, MEM_RELEASE);
}
