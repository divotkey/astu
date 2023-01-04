/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Extensions/CompiledSource.h"

namespace astu {

    class GraphicsInstants : public CompiledSource {
    public:
    protected:
        const unsigned char *GetData() const override;

        size_t GetDataSize() const override;

    private:
        static const unsigned char data[];
        static const size_t size;
    };

} // end of namespace
