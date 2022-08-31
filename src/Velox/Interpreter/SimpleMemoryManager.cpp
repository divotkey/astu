// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "Velox/Interpreter/SimpleMemoryManager.h"

namespace velox {

    SimpleMemoryManager::SimpleMemoryManager()
        : totalAllocations(0)
        , totalFrees(0)
        , curAllocations(0)
        , maxAllocations(0)
        , curMemory(0)
        , maxMemory(0)
    {
        // Intentionally left empty.
    }

    void *velox::SimpleMemoryManager::Allocate(size_t size) {
        ++totalAllocations;
        ++curAllocations;
        curMemory += size;
        if (curAllocations > maxAllocations) {
            maxAllocations = curAllocations;
        }
        if (curMemory > maxMemory) {
            maxMemory = curMemory;
        }
        return new unsigned char[size];
    }

    void velox::SimpleMemoryManager::Free(void *pVoid) {
        ++totalFrees;
        --curAllocations;
        //curMemory -=
        delete [] static_cast<unsigned char*>(pVoid);
    }

    unsigned int SimpleMemoryManager::GetNumAllocations() const {
        return totalAllocations;
    }

    unsigned int SimpleMemoryManager::GetNumFrees() const {
        return totalFrees;
    }

    unsigned int SimpleMemoryManager::GetMaxAllocations() const {
        return maxAllocations;
    }

    size_t SimpleMemoryManager::GetMaxMemory() const {
        return maxMemory;
    }

    unsigned int SimpleMemoryManager::GetCurrentAllocations() const {
        return curAllocations;
    }

}
