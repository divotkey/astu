/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

#include <cstddef>
#include "Velox/Interpreter/IMemoryManager.h"

namespace velox {

    class MemoryManager : public IMemoryManager {
    public:

        MemoryManager();

        // Inherited via IMemoryManager
        void *Allocate(size_t size) override;
        void Free(void *pVoid) override;
        unsigned int GetNumAllocations() const override;
        unsigned int GetNumFrees() const override;
        unsigned int GetMaxAllocations() const override;
        size_t GetMaxMemory() const override;
        unsigned int GetCurrentAllocations() const override;
        size_t GetFreeMemory() const override;
        size_t GetAvailableMemory() const override;

    private:
        struct Block {
            size_t size;
            Block* next;
        };

        Block* head;
        void* data;
        unsigned totalAllocations;
        unsigned totalFrees;
        unsigned curAllocations;
        unsigned maxAllocations;
        size_t curMemory;
        size_t maxMemory;

        Block* FindMatching(size_t size);
        Block* SplitMemory(size_t size);
    };

}



