/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local (AST-Utilities) includes
#include "Vector2.h"
#include "Color.h"

namespace astu {

    /**
     * Interface for simple line rendering.
     * 
     * @ingroup gfx_group
     */
    class ILineRenderer {
    public:

        /**
         * Virtual Destructor.
         */
        ~ILineRenderer() {}

        /**
         * Draws a line between two points.
         * 
         * @parma p1    the first point of the line
         * @parma p2    the second point of the line
         */
        virtual void DrawLine(const Vector2<double> & p1, const Vector2<double> & p2) {
            DrawLine(p1.x, p1.y, p2.x, p2.y);
        }

        /**
         * Draws a line between two points.
         * 
         * @parma x1    the x-coordinate of the first point of the line
         * @parma y1    the y-coordinate of the first point of the line
         * @parma x2    the x-coordinate of the second point of the line
         * @parma y2    the y-coordinate of the second point of the line
         */
        virtual void DrawLine(double x1, double y1, double x2, double y2) = 0;

        /**
         * Sets the current drawing color used for all subsequent drawing calls.
         * 
         * @param c the new drawing color 
         */
        virtual void SetDrawColor(const Color & c) = 0;
    };

} // end of namespace