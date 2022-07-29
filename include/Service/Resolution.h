/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

namespace astu {

    /**
     * Describes common resolutions for application XOS.
     * 
     * @ingroup srv_group
     */
    enum class Resolution {
        /** Aspect ratio 4:3, pixels 800 x 600. */
        SVGA,

        /** Aspect ratio 4:3, pixels 1024 x 768. */
        XGA,

        /** Aspect ratio 16:9, pixels 1280 x 720. */
        WXGA,

        /** Aspect ratio 5:4, pixels 1280 x 1024. */
        SXGA, 

        /** Aspect ratio ~16:9, pixels 1360 x 768. */
        HD_1,

        /** Aspect ratio ~16:9, pixels 1366 x 768. */
        HD_2,

        /** Aspect ratio 16:10, pixels 1440 x 900. */
        WXGA_PLUS,

        /** Aspect ratio 16:10, pixels 1680 x 1050. */
        WSXGA_PLUS,

        /** Aspect ratio 16:9, pixels 1920 x 1080. */
        FHD,

        /** Aspect ratio 16:9, pixels 2560 x 1440. */
        QHD, 

        /** Aspect ratio 21:9, pixels 3440 x 1440. */
        WIDE_SCREEN, 

        /** Aspect ratio 16:9, pixels 3840 x 2160. */
        UHD_4K, 
    };

} // end of namespace
