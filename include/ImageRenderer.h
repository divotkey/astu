/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 * 
 *----------------------------------------------------------------------------
 * Meanings of ASTU: Slightly melted butter (Hinduism / Religious Law), 
 * Let it be / Be it so (Sanskrit), City (Ancient Greek)
 *----------------------------------------------------------------------------
 */

// Standard C++ Library includes
#include <memory>

// ASTU Library includes
#include "RenderQuality.h"
#include "Image.h"
#include "Color.h"
#include "Vector2.h"

namespace astu {

    // Forward declaration
    class UnionPattern;
    class UnicolorPattern;
    class Quadtree;
    class IPatternRenderer;

    class ImageRenderer final {
    public:

        /**
         * Constructor.
         */
        ImageRenderer();

        /**
         * Destructor.
         */
        ~ImageRenderer();


        void Clear();
        void SetDrawColor(const Color & c);
        void SetBackgroundColor(const Color & c);

        void DrawCircle(double x, double y, double r);


        void DrawLine(const Vector2<double> &p0, const Vector2<double> & p1, double w = 1) {
            DrawLine(p0.x, p0.y, p1.x, p1.y, w);
        }

        void DrawLine(double x0, double y0, double x1, double y1, double w = 1);

        /**
         * Draws a rectangle with the current draw color.
         * 
         * @param cx    the the x-coordinate of the rectangle's center
         * @param cy    the the y-coordinate of the rectangle's center
         * @param x     the width of the rectangle
         * @param h     the height of the rectangle
         * @param phi   the rotation angle in degrees
         */
        void DrawRectangle(double cx, double cy, double w, double h, double phi = 0);

        /**
         * Draws a rectangle with the current draw color.
         * 
         * @param center    the center of the rectangl
         * @param x     the width of the rectangle
         * @param h     the height of the rectangle
         * @param phi   the rotation angle in degrees
         */
        void DrawRectangle(const Vector2<double> & center, double w, double h, double phi = 0) {
            DrawRectangle(center.x, center.y, w, h, phi);
        }

        /**
         * Sets the render quality used to create the image.
         * 
         * @param quality   the render quality
         */
        void SetRenderQuality(RenderQuality quality);

        /**
         * Returns the currently set render quality.
         * 
         * @return quality
         */
        RenderQuality GetRenderQuality() const {
            return quality;
        }

        /**
         * Renders the image.
         * 
         * @param img   image used to store the resulting rendering
         */
        void Render(Image & img);

    private:
        /** The render quality. */
        RenderQuality quality;

        /** The current drawing color. */
        Color drawColor;

        /** The background color. */
        Color backgroundColor;

        /** The root container for all patterns. */
        std::unique_ptr<UnionPattern> root;

        /** Represents the background color. */
        std::shared_ptr<UnicolorPattern> background;

        /** Contains all drawing elements. */
        std::shared_ptr<Quadtree> quadtree;

        /** Used to render the image. */
        std::unique_ptr<IPatternRenderer> renderer;
    };

} // end of namespace