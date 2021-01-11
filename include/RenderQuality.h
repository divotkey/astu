/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

namespace astu {

    /**
     * Describes different levels of render quality.
     */
    enum class RenderQuality { 
            Fast, 
            Simple,
            Good,
            Beautiful,
            Insane
    };

    /**
     * Describes different levels of anti-aliasing.
     */
    enum class AntialiasingLevel {
            Simple,
            Good,
            Beautiful,
            Insane
    };

} // end of namespace

