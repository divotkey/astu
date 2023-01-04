/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/Service.h"
#include "Math/Polygon.h"
#include "Math/Vector2.h"
#include "Math/Matrix3.h"
#include "Graphics/Color.h"
#include "Graphics/WebColors.h"

// C++ Standard Library includes
#include <stack>
#include <vector>

namespace astu::suite2d {

    /**
     * Base class for 2D line renderers.
     * 
     * @ingroup suite2d_group
     */
    template <typename T>
    class LineRenderer {
    public:

        /**
         * Constructor.
         */
        LineRenderer()
            : mvDirty(true)
            , color(WebColors::Aqua)
        {
            // Intentionally left empty.
        }

        /**
         * Virtual Destructor.
         */
        ~LineRenderer() {}

        /**
         * Sets the current drawing color used for all subsequent draw calls.
         * 
         * @param c the new drawing color 
         */
        void SetDrawColor(const Color<T>& c) {
            color = c;
            OnSetDrawColor(c);
        }

        /**
         * Returns the current draw color.
         * 
         * @return the current draw color
         */
        const Color<T>& GetDrawColor() const {
            return color;
        }

        /**
         * Sets the transformation matrix used for rendering.
         * 
         * @param m the transformation matrix
         */
        void SetTransform(const Matrix3<T>& m) {
            modelTransform = m;
            mvDirty = true;
        }

        /**
         * Returns the current transformation matrix used for rendering.
         * 
         * @return the transformation matrix
         */
        const Matrix3<T>& GetTransform() const {
            return modelTransform;
        }

        /**
         * Resetns the current transfomation to identity.
         */
        void ResetTransform() {
            modelTransform.SetToIdentity();
            mvDirty = true;
        }

        /**
         * Sets the view transformation matrix.
         * 
         * @param view  the transformation matrix
         */
        void SetViewTransform(const Matrix3<T>& view) {
            viewTransform = view;
            mvDirty = true;
        }

        /**
         * Returns the view matrix.
         * 
         * @return the view matrix
         */
        const Matrix3f& GetViewTransform() const {
            return viewTransform;
        }

        /**
         * Draws a line between two points.
         * 
         * @param x1    the x-coordinate of the first point of the line
         * @param y1    the y-coordinate of the first point of the line
         * @param x2    the x-coordinate of the second point of the line
         * @param y2    the y-coordinate of the second point of the line
         */
        virtual void DrawLine(T x1, T y1, T x2, T y2) = 0;

        /**
         * Draws a line between two points.
         * 
         * @param p1    the first point of the line
         * @param p2    the second point of the line
         */
        void DrawLine(const Vector2<T> & p1, const Vector2<T> & p2) {
            DrawLine(p1.x, p1.y, p2.x, p2.y);
        }

        /**
         * Draw a series of connected lines.
         * 
         * @param vertices  the points along the lines
         */
        void DrawLines(const std::vector<Vector2<T>>& vertices) {
            if (vertices.size() < 2) 
                return;

            auto p0 = vertices.begin();
            for (auto p1 = vertices.begin() + 1; p1 < vertices.end(); ++p0, ++p1) {
                DrawLine(*p0, *p1);
            }
        }

        /**
         * Draws an axis-aligned rectangle.
         * 
         * @param c the center of the rectangle
         * @param s the dimensions of the rectangle
         */
        void DrawRectangle(const Vector2<T>& c, const Vector2<T>& s) {
            DrawRectangle(c.x, c.y, s.x, s.y);
        }

        /**
         * Draws an axis-aligned rectangle.
         * 
         * @param cx    the x-coordinate of the center of the rectangle
         * @param cy    the y-coordinate of the center of the rectangle
         * @param w     the width of the rectangle
         * @param h     the height of the rectangle
         */
        virtual void DrawRectangle(T cx, T cy, T w, T h) {
            DrawLine(cx - w / 2, cy - h / 2, cx + w / 2, cy - h / 2);
            DrawLine(cx - w / 2, cy - h / 2, cx - w / 2, cy + h / 2);
            DrawLine(cx + w / 2, cy - h / 2, cx + w / 2, cy + h / 2);
            DrawLine(cx + w / 2, cy + h / 2, cx - w / 2, cy + h / 2);
        }

        /**
         * Draws a circle.
         * 
         * @param c         the center of the circle
         * @param r         the radius of the circle
         * @param segments  the number of line segments used to draw the circle
         */
        virtual void DrawCircle(Vector2<T> c, T r, unsigned int segments = 24) {
            DrawCircle(c.x, c.y, r, segments);
        }

        /**
         * Draws a circle.
         * 
         * @param cx        the x-coordinate of the center of the circle
         * @param cy        the y-coordinate of the center of the circle
         * @param r         the radius of the circle
         * @param segments  the number of line segments used to draw the circle
         */
        virtual void DrawCircle(T cx, T cy, T r, unsigned int segments = 24) {
            T da = static_cast<T>(MathUtils::PI2d) / segments;

            Vector2<T> p0(0, r);
            for (unsigned int i = 0; i < segments; ++i) {
                Vector2<T> p1(0, r);
                p1.Rotate(i * da);

                DrawLine(p0.x + cx, p0.y + cy, p1.x + cx, p1.y + cy);
                p0 = p1;
            }
            DrawLine(p0.x + cx, p0.y + cy, cx, r + cy);
        }

        /**
         * Draws a polygon.
         * 
         * @param poly  the polygon to draw
         */
        virtual void DrawPolygon(const Polygon<T>& poly) {
            if (poly.NumVertices() < 3) { 
                // Degenerated polygon
                return;
            }

            const auto& vertices = poly.GetVertices();
            auto p0 = vertices.begin();
            for (auto p1 = vertices.begin() + 1; p1 < vertices.end(); ++p0, ++p1) {
                DrawLine(*p0, *p1);
            }
            DrawLine(*p0, vertices.front());
        }

        /**
         * Draws a polygon.
         * 
         * @param poly          the polygon to draw
         * @param normalScale   the scaling factor used to render the normals
         */
        virtual void DrawPolygonNormals(const Polygon<T>& poly, T normalScale)  {
            if (poly.NumVertices() < 3) { 
                // Degenerated polygon
                return;
            }

            for (size_t i = 0; i < poly.NumEdges(); ++i) {
                const auto& p = poly.GetEdgeCenter(i);
                DrawLine(p, p + poly.GetEdgeNormal(i) * normalScale);
            }
        }

    protected:

        /**
         * Called when a new color is set.
         * 
         * @param color the new color
         */
        virtual void OnSetDrawColor(const Color<T>& color) { }

        /**
         * Returns the current model-view transformation matrix.
         * 
         * @return the model-view transformation matrix
         */
        const Matrix3<T>& GetModelViewMatrix() {
            if (mvDirty) {
                mv = viewTransform * modelTransform;
                mvDirty = false;
            }
            return mv;
        }
        /**
         * Draws an axis-aligned rectangle.
         * 
         * @param c the center of the rectangle
         * @param s the dimensions of the rectangle
         */

    private:
        /** The current color used for rendering. */
        Color<T> color;

        /** The model transformation matrix. */
        Matrix3<T> modelTransform;

        /** The view transformation matrix. */
        Matrix3<T> viewTransform;

        /** The model-view transformation matrix. */
        Matrix3<T> mv;

        /** Indicates that the model-view matrix is dirty. */
        bool mvDirty;

    };

    /**
     * Type alias for ILineRenderer2f template using double as data type.
     */
    using ILineRenderer2d = LineRenderer<double>;

    /**
     * Type alias for ILineRenderer2f template using float as data type.
     */
    using ILineRenderer2f = LineRenderer<float>;

    /**
     * Serices can derive from this class to use a line renderer in a 
     * convenient way. This class fetches a line renderer and also 
     * replicates its method for convenient usage. It also provides a 
     * matrix transformation stack.
     * 
     * @tparam T    the numerical data type used for rendering
     */
    template <typename T>
    class LineRendererClient : public virtual Service {
    public:

        /**
         * Constructor.
         */
        LineRendererClient(){
            AddStartupHook([this](){ 
                lineRenderer = ASTU_GET_SERVICE(LineRenderer<T>); 
            });

            AddShutdownHook([this](){ lineRenderer = nullptr;
            });
        }

    protected:

        /**
         * Returns the line renderer.
         * 
         * @return the line renderer
         */
        LineRenderer<T> & GetLineRenderer() {
            return *lineRenderer;
        }


        /**
         * Sets the view tranformation. 
         * 
         * The view transformation matrix is typically provieded by a camera.
         * 
         * @param m the view transformation matrix
         */
        void SetViewTransform(const Matrix3<T>& m) {
            lineRenderer->SetViewTransform(m);
        }

        /**
         * Resetns the current transfomation to identity.
         */
        void ResetTransform() {
            lineRenderer->ResetTransform();
        }

        /**
         * Sets the transformation to the identity.
         */
        void Identity() {
            lineRenderer->SetTransform(Matrix3<T>());
        }

        /**
         * Sets the transformation matrix.
         * 
         * @param m the transformation matrix to become the current one
         */
        void SetTransform(const Matrix3<T>& m) {
            lineRenderer->SetTransform(m);
        }

        /**
         * Returns the current transformation matrix.
         * 
         * @return the transformation matrix currently used.
         */
        const Matrix3<T>& GetTransform() const {
            return lineRenderer->GetTransform();
        }

        /**
         * Sets the transformation a translation.
         * 
         * @param x the x-coordinate of the translation
         * @param y the y-coordinate of the translation
         */
        void SetTranslation(float x, float y) {
            lineRenderer->SetTransform(Matrix3f().SetToTranslate(x, y));
        }

        /**
         * Sets the transformation a translation.
         * 
         * @param x the x-coordinate of the translation
         * @param y the y-coordinate of the translation
         */
        void SetTranslation(const Vector2<T>& t) {
            lineRenderer->SetTransform(Matrix3f().SetToTranslate(t));
        }

        /**
         * Adds a translation to the current transfromation.
         * 
         * @param dx    the delta movment on the x-axis
         * @param dy    the delta movment on the y-axis
         */
        void Translate(float dx, float dy) {
            auto tx = lineRenderer->GetTransform();
            tx.Translate(dx, dy);
            lineRenderer->SetTransform(tx);
        }

        /**
         * Adds a translation to the current transfromation.
         * 
         * @param delta the delta movement
         */
        void Translate(const Vector2<T>& delta) {
            auto tx = lineRenderer->GetTransform();
            tx.Translate(delta);
            lineRenderer->SetTransform(tx);
        }

        /**
         * Adds a rotation to the current transfromation.
         * 
         * @param deltaPhi  the rotation angle in radians
         */
        void Rotate(const Vector2<T>& deltaPhi) {
            auto tx = lineRenderer->GetTransform();
            tx.Rotate(deltaPhi);
            lineRenderer->SetTransform(tx);
        }

        /**
         * Adds a rotation to the current transfromation.
         * 
         * @param deltaPhi  the rotation angle in degrees
         */
        void RotateDeg(const Vector2<T>& deltaPhi) {
            auto tx = lineRenderer->GetTransform();
            tx.RotateDeg(deltaPhi);
            lineRenderer->SetTransform(tx);
        }

        /**
         * Adds a scaling to the corrent transformation.
         * 
         * @param deltaScale    the scaling factors in x and y dimensions
         */
        void Scale(const Vector2<T>& deltaScale) {
            auto tx = lineRenderer->GetTransform();
            tx.Scale(deltaScale);
            lineRenderer->SetTransform(tx);
        }

        /**
         * Pushes the current transform on the transfomation stack.
         */
        void PushTransform() {
            transformStack.push(lineRenderer->GetTransform());
        }

        /**
         * Restores the last pushed transform from the transformation stack.
         */
        void PopTransform() {
            lineRenderer->SetTransform(transformStack.top());
            transformStack.pop();
        }

        /**
         * Sets the current drawing color used for all subsequent draw calls.
         * 
         * @param c the new drawing color 
         */
        void SetDrawColor(const Color<T> & c) {
            lineRenderer->SetDrawColor(c);
        }

        /**
         * Returns the current draw color.
         * 
         * @return the current draw color
         */
        const Color<T>& GetDrawColor() {
            return lineRenderer->GetDrawColor();
        }

        /**
         * Draws a line between two points.
         * 
         * @param p1    the first point of the line
         * @param p2    the second point of the line
         */
        virtual void DrawLine(const Vector2<T> & p1, const Vector2<T> & p2) {
            lineRenderer->DrawLine(p1.x, p1.y, p2.x, p2.y);
        }     

        /**
         * Draws a line between two points.
         * 
         * @param x1    the x-coordinate of the first point of the line
         * @param y1    the y-coordinate of the first point of the line
         * @param x2    the x-coordinate of the second point of the line
         * @param y2    the y-coordinate of the second point of the line
         */
        void DrawLine(T x1, T y1, T x2, T y2) {
            lineRenderer->DrawLine(x1, y1, x2, y2);
        }

        /**
         * Draw a series of connected lines.
         * 
         * @param vertices  the points along the lines
         */
        void DrawLines(const std::vector<Vector2<T>>& vertices) {
            lineRenderer->DrawLines(vertices);
        }

        /**
         * Draws an axis-aligned rectangle.
         * 
         * @param cx    the x-coordinate of the center of the rectangle
         * @param cy    the y-coordinate of the center of the rectangle
         * @param w     the width of the rectangle
         * @param h     the height of the rectangle
         */
        void DrawRectangle(T cx, T cy, T w, T h) {
            lineRenderer->DrawRectangle(cx, cy, w, h);
        }

        /**
         * Draws an axis-aligned rectangle.
         * 
         * @param c the center of the rectangle
         * @param s the dimensions of the rectangle
         */
        void DrawRectangle(const Vector2<T>& c, const Vector2<T>& s) {
            lineRenderer->DrawRectangle(c, s);
        }

        /**
         * Draws a circle.
         * 
         * @param cx        the x-coordinate of the center of the circle
         * @param cy        the y-coordinate of the center of the circle
         * @param r         the radius of the circle
         * @param segments  the number of line segments used to draw the circle
         */
        void DrawCircle(T cx, T cy, T r, unsigned int segments = 24) {
            lineRenderer->DrawCircle(cx, cy, r, segments);
        }

        /**
         * Draws a circle.
         * 
         * @param c         the center of the circle
         * @param r         the radius of the circle
         * @param segments  the number of line segments used to draw the circle
         */
        void DrawCircle(const Vector2<T>& c, T r, unsigned int segments = 24) {
            DrawCircle(c.x, c.y, r, segments);
        }

        /**
         * Draws a polygon.
         * 
         * @param poly  the polygon to draw
         */
        void DrawPolygon(const Polygon<T>& poly) {
            lineRenderer->DrawPolygon(poly);
        }

        /**
         * Draws the normals of a polygon.
         * 
         * @param poly          the polygon
         * @param normalScale   the scaling factor used to render the normals
         */
        void DrawPolygonNormals(const Polygon<T>& poly, T normalScale) {
            lineRenderer->DrawPolygonNormals(poly, normalScale);
        }

    private:
        /** The line renderer. */
        std::shared_ptr<LineRenderer<T>> lineRenderer;

        /** Used to push and pop transformations. */
        std::stack<Matrix3<T>> transformStack;
    };

    /**
     * Type alias for LineRendererClient template using float as data type.
     */
    using LineRendererClient2f = LineRendererClient<float>;

    /**
     * Type alias for LineRendererClient template using double as data type.
     */
    using LineRendererClient2d = LineRendererClient<double>;
    
} // end of namespace