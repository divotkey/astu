#include "MemoryManager.h"

#include <cassert>
#include <stdexcept>
#include <iostream>
//#include <new>

#define KILOBYTE    1024
#define MEGABYTE    1024 * KILOBYTE
#define GIGABYTE    1024 * MEGABYTE
#define MEM_SIZE    5 * 1024

using namespace std;

namespace velox {

    IMemoryManager* gMemoryManager = new MemoryManager;

    MemoryManager::MemoryManager()
        : totalAllocations(0)
        , totalFrees(0)
        , curAllocations(0)
        , maxAllocations(0)
        , curMemory(0)
        , maxMemory(0)
    {
        data = new unsigned char[MEM_SIZE];
        head = reinterpret_cast<Block*>(data);
        head->size = MEM_SIZE;
        head->next = nullptr;
    }

    void *velox::MemoryManager::Allocate(size_t size) {

        size_t actualSize = size + sizeof(Block);
        auto block = FindMatching(actualSize);
        if (!block) {
            block = SplitMemory(actualSize);
        }
        assert(block);

        ++totalAllocations;
        ++curAllocations;
        curMemory += actualSize;
        if (curAllocations > maxAllocations) {
            maxAllocations = curAllocations;
        }
        if (curMemory > maxMemory) {
            maxMemory = curMemory;
        }

        return reinterpret_cast<unsigned char*>(block) + sizeof(Block);
    }

    void velox::MemoryManager::Free(void *pVoid) {
        Block* block = reinterpret_cast<Block*>(reinterpret_cast<unsigned char*>(pVoid) - sizeof(Block));

        ++totalFrees;
        --curAllocations;
        if (curMemory < block->size) {
            throw runtime_error("Corrupted memory management");
        }
        curMemory -= block->size;

        block->next = head;
        head = block;
    }

    unsigned int MemoryManager::GetNumAllocations() const {
        return totalAllocations;
    }

    unsigned int MemoryManager::GetNumFrees() const {
        return totalFrees;
    }

    unsigned int MemoryManager::GetMaxAllocations() const {
        return maxAllocations;
    }

    size_t MemoryManager::GetMaxMemory() const {
        return maxMemory;
    }

    unsigned int MemoryManager::GetCurrentAllocations() const {
        return curAllocations;
    }

    MemoryManager::Block *MemoryManager::FindMatching(size_t size) {
        Block **prevNext = &head;
        Block *cur = head;
        while (cur && cur->size != size) {
            prevNext = &cur->next;
            cur = cur->next;
        }

        if (!cur) {
            return nullptr;
        }

        *prevNext = cur->next;
        cur->next = nullptr;

        return cur;
    }

    MemoryManager::Block *MemoryManager::SplitMemory(size_t size) {
        Block **prevNext = &head;
        auto cur = head;

        // We need a block, that can be split and still has free space for a block header.
        size_t minSize = size + sizeof(Block);
        while (cur && cur->size <= minSize) {
            prevNext = &cur->next;
            cur = cur->next;
        }

        if (!cur) {
            //throw bad_alloc();
            throw runtime_error("Out of interpreter memory");
        }

        Block *newFree = reinterpret_cast<Block*>(reinterpret_cast<unsigned char*>(cur) + size);
        *prevNext = newFree;

        assert(cur->size - size > sizeof(Block));
        newFree->size = cur->size - size;
        newFree->next = cur->next;

        cur->size = size;
        cur->next = nullptr;

        return cur;
    }

    size_t MemoryManager::GetFreeMemory() const {
        size_t sum = 0;
        Block* cur = head;
        while (cur) {
            sum += cur->size;
            cur = cur->next;
        }

        return sum;
    }

    size_t MemoryManager::GetAvailableMemory() const {
        size_t sum = 0;
        Block* cur = head;
        while (cur) {
            sum += cur->size - sizeof(Block);
            cur = cur->next;
        }

        return sum;
    }

}
