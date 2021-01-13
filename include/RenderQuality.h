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
     * 
     * @ingroup gfx_group
     */
    enum class RenderQuality {
            /** Fastest rendering, no anti-aliasing etc. */
            Fast, 

            /** Fast rendering, some anti-aliasing, average quality. */
            Simple,

            /** Compromise between quality and speed, good anti-aliasing. */
            Good,

            /** Use for high quality renderings, final releases, etc. */
            Beautiful,

            /** Very slow rendering but highest output quality (placebo?). */
            Insane
    };

    /**
     * Describes different levels of anti-aliasing.
     * 
     * @ingroup gfx_group
     */
    enum class AntialiasingLevel {

            /** Very simple anti-aliasing using a 3x3 kernel. */
            Simple,

            /** Good anti-aliasing using a 5x5 kernel. */
            Good,

            /** Very good anti-aliasing using a 7x7 kernel. */
            Beautiful,

            /** Slow anti-aliasing using a 9x9 kernel. */
            Insane
    };

} // end of namespace

