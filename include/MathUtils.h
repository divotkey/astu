/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

namespace astu {

    template <typename T>
    T ToRadians(T phi) {
        return phi * static_cast<T>(3.14159265358979323846 / 180.0);
    }

    template <typename T>
    T ToDegrees(T phi) {
        return phi * static_cast<T>(180.0 / 3.14159265358979323846);
    }

} // end of namespace