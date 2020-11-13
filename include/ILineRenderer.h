/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include "Vector2.h"

namespace astu {

    class ILineRenderer {
    public:

        /**
         * Virtual Destructor.
         */
        ~ILineRenderer();

        /**
         * Draws a line between two points.
         * 
         * @parma p1    the first point of the line
         * @parma p2    the second point of the line
         */
        virtual void DrawLine(const Vector2 & p1, const Vector2 & p2) = 0;
    };

}