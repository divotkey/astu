// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

#include <cstddef>
#include "Velox/Interpreter/IMemoryManager.h"

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



