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

// C++ Standard Library includes
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

    /**
     * This class can be used to render geometric primitives in an resolution independent way.
     * The output of the rendering will be an Image.
     * 
     * **Example Usage**
     * 
     * ```
     * #include <stdexcept>
     * #include <iostream>
     * #include <AstGraphics.h>
     * 
     * using namespace std;
     * using namespace astu;
     * 
     * int main()
     * {
     *   const int kWidth = 512;
     *   const int kHeight = 512;
     * 
     *   ImageRenderer renderer;
     * 
     *   // Create some graphical elements.
     *   renderer.SetDrawColor(Color(1.0, 0.8, 0.5, 0.5));
     *   renderer.DrawRectangle(kWidth / 2.0, kHeight / 2.0, kHeight, 50);
     * 
     *   renderer.SetDrawColor(WebColors::Red);
     *   renderer.DrawCircle(kWidth / 2.0, kHeight / 2.0, 100.0);
     * 
     *   renderer.SetDrawColor(Color(1.0, 0.8, 0.5, 0.5));
     *   renderer.DrawRectangle(kWidth / 2.0, kHeight / 2.0, 50, kWidth);
     * 
     *   renderer.SetDrawColor(WebColors::Green);
     *   renderer.DrawRectangle(kWidth / 4.0, kHeight / 4.0, 50, 50, 45);
     * 
     *   // Background can be defined at any time.
     *   renderer.SetBackgroundColor(WebColors::White);
     *
     * 
     *   // Render Image 
     *   Image image(kWidth, kHeight);
     *   renderer.SetRenderQuality(RenderQuality::Good);
     *   std::cout << "Rendering image" << std::endl;
     *   renderer.Render(image);
     * 
     *   // Store image to file, might cause an exception in case
     *   // of an I/O error.
     *   try {
     *     StoreImage(image, "output.bmp");
     *   } catch (std::runtime_error & e) {
     *     std::cout << "Error storing image: " << e.what() << std::endl;
     *     return -1;
     *   }
     * 
     *   return 0;
     * }     
     * ```     
     * 
     * This code will generate this image:
     * 
     * \image html rendered_image.png
     * 
     * @ingroup gfx_group
     */
    class ImageRenderer final {
    public:

        /**
         * Constructor.
         * 
         * @param the maximum recursion depth of the scene quad tree
         */
        ImageRenderer(unsigned int maxDepth = 10);

        /**
         * Destructor.
         */
        ~ImageRenderer();

        /**
         * Clear the output rendering.
         */
        void Clear();

        /**
         * Sets the current draw color.
         * 
         * Subsequent rendering calls will be using the current draw color.
         * 
         * @param c the color
         */
        void SetDrawColor(const Color & c) noexcept;

        /**
         * Returns the background color
         * 
         * @return the current background color
         */
        const Color & GetDrawColor() const noexcept;

        /**
         * Sets the background color.
         * 
         * 
         * The background color can be set anytime, even after rendering
         * of graphical elements.
         * 
         * @param c the background color
         */
        void SetBackgroundColor(const Color & c) noexcept;

        /**
         * Returns the background color
         * 
         * @return the current background color
         */
        const Color & GetBackgroundColor() const noexcept;

        /**
         * Draws a filled circle.
         * 
         * @param cx    the x-coordinate of the center of the circle
         * @param cy    the y-coordinate of the center of the circle
         * @param r     the radius of the circle
         */
        void DrawCircle(double cx, double cy, double r);

        /**
         * Draws a filled circle.
         * 
         * @param c the center of the circle
         * @param r the radius of the circle
         */
        void DrawCircle(const Vector2<double> &c, double r)
        {
            DrawCircle(c.x, c.y, r);
        }

        /**
         * Draws a line.
         * 
         * @param p0    the start point of the line
         * @param p1    the end point of the line
         * @param w     the width of the line
         */
        void DrawLine(const Vector2<double> &p0, const Vector2<double> & p1, double w = 1) {
            DrawLine(p0.x, p0.y, p1.x, p1.y, w);
        }

        /**
         * Draws a line.
         * 
         * @param x0    the x-coordinate of the start point of the line
         * @param y0    the y-coordinate of the start point of the line
         * @param x1    the x-coordinate of the end point of the line
         * @param y1    the y-coordinate of the end point of the line
         * @param w     the width of the line
         */
        void DrawLine(double x0, double y0, double x1, double y1, double w = 1);

        /**
         * Draws a filled rectangle.
         * 
         * @param cx    the the x-coordinate of the rectangle's center
         * @param cy    the the y-coordinate of the rectangle's center
         * @param w     the width of the rectangle
         * @param h     the height of the rectangle
         * @param angleDeg  the rotation angle in degrees
         */
        void DrawRectangle(double cx, double cy, double w, double h, double angleDeg = 0);

        /**
         * Draws a rectangle with the current draw color.
         * 
         * @param center    the center of the rectangle
         * @param w         the width of the rectangle
         * @param h         the height of the rectangle
         * @param angleDeg  the rotation angle in degrees
         */
        void DrawRectangle(const Vector2<double> & center, double w, double h, double angleDeg = 0) {
            DrawRectangle(center.x, center.y, w, h, angleDeg);
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
         * Returns the maximum recursion depth of the scene quadtree.
         * 
         * @return depth of scene quadtree
         */
        unsigned int GetQuadtreeDepth() const {
            return quadtreeDepth;
        }

        /**
         * Sets the maximum recursion depth of the scene quadtree.
         * 
         * Note: changing the depth will only have on effect after Clear() 
         * has been called.
         * 
         * @param depth the depth of the scene quadtree.
         */
        void SetQuadtreeDepth(unsigned int);

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

        /** The maximum recursion depth for the scene quadtree. */
        unsigned int  quadtreeDepth;
    };

} // end of namespace