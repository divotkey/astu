/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Math/MathUtils.h"

namespace astu {

    const double MathUtils::PId = 3.14159265358979323846;
    const double MathUtils::PI2d = PId * 2;
    const double MathUtils::PIOver2d = PId / 2;
    const double MathUtils::PIOver4d = PId / 4;
    const double MathUtils::InvPId = 1.0 / PId;
    const double MathUtils::InvPI2d = 1.0 / (2 * PId);
    const double MathUtils::Epsilon = 10e-9;

    const float MathUtils::PIf = static_cast<float>(PId);
    const float MathUtils::PI2f = static_cast<float>(PI2d);
    const float MathUtils::PIOver2f = static_cast<float>(PIOver2d);
    const float MathUtils::PIOver4f = static_cast<float>(PIOver4d);
    const float MathUtils::InvPIf = static_cast<float>(InvPId);
    const float MathUtils::InvPI2f = static_cast<float>(InvPI2d);

    size_t MathUtils::GetIndex(int i, size_t m)
    {
        int idx = i % static_cast<int>(m);
        if (idx < 0)
            idx += static_cast<int>(m);

        return static_cast<size_t>(idx);
    }

} // end of namespace
