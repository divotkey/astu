#pragma once

#include <cstddef>
#include "IMemoryManager.h"

namespace velox {

    class SimpleMemoryManager : public IMemoryManager {
    public:

        SimpleMemoryManager();

        // Inherited via IMemoryManager
        void *Allocate(size_t size) override;
        void Free(void *pVoid) override;
        unsigned int GetNumAllocations() const override;
        unsigned int GetNumFrees() const override;
        unsigned int GetMaxAllocations() const override;
        size_t GetMaxMemory() const override;

        unsigned int GetCurrentAllocations() const override;

    private:
        unsigned totalAllocations;
        unsigned totalFrees;
        unsigned curAllocations;
        unsigned maxAllocations;
        size_t curMemory;
        size_t maxMemory;
    };

}



