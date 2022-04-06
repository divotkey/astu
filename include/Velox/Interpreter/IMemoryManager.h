#pragma once

namespace velox {

    class IMemoryManager {
    public:
        virtual void* Allocate(size_t) = 0;
        virtual void Free(void*) = 0;
        virtual unsigned int GetNumAllocations() const = 0;
        virtual unsigned int GetNumFrees() const = 0;
        virtual unsigned int GetMaxAllocations() const = 0;
        virtual unsigned int GetCurrentAllocations() const = 0;
        virtual size_t GetMaxMemory() const = 0;
        virtual size_t GetFreeMemory() const = 0;
        virtual size_t GetAvailableMemory() const = 0;
    };

    extern IMemoryManager* gMemoryManager;
}
