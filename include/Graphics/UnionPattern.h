/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Graphics/CompoundPattern.h"

// C++ Standard Library includes
#include <memory>
#include <vector>

namespace astu {

    class UnionPattern : public CompoundPattern {
    public:

    protected:
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d & c) const override;
        virtual BoundingBox GetLocalBoundingBox() const override;

    private:
        void Blend(Color4d & a, const Color4d & b) const;
    };

}