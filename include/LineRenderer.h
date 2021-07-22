/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/Service.h"
#include "Vector2.h"
#include "Matrix3.h"
#include "Color.h"
#include "Math/Polygon.h"

// C++ Standard Library includes
#include <stack>

namespace astu {

    /**
     * Base class for 2D line renderers.
     * 
     * @ingroup gfx_group
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
         * Sets the current drawing color used for all subsequent drawing calls.
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

        /***
         * Returns the view matrix.
         * 
         * @return the view matrix
         */
        const Matrix3f& SetViewTransform() const {
            return viewMatrix;
        }

        /**
         * Draws a line between two points.
         * 
         * @parma x1    the x-coordinate of the first point of the line
         * @parma y1    the y-coordinate of the first point of the line
         * @parma x2    the x-coordinate of the second point of the line
         * @parma y2    the y-coordinate of the second point of the line
         */
        virtual void DrawLine(T x1, T y1, T x2, T y2) = 0;

        /**
         * Draws a line between two points.
         * 
         * @parma p1    the first point of the line
         * @parma p2    the second point of the line
         */
        void DrawLine(const Vector2<T> & p1, const Vector2<T> & p2) {
            DrawLine(p1.x, p1.y, p2.x, p2.y);
        }

        void DrawRectangle(const Vector2<T>& c, const Vector2<T>& s) {
            DrawRectangle(c.x, c.y, s.x, s.y);
        }

        virtual void DrawRectangle(T cx, T cy, T w, T h) {
            DrawLine(cx - w / 2, cy - h / 2, cx + w / 2, cy - h / 2);
            DrawLine(cx - w / 2, cy - h / 2, cx - w / 2, cy + h / 2);
            DrawLine(cx + w / 2, cy - h / 2, cx + w / 2, cy + h / 2);
            DrawLine(cx + w / 2, cy + h / 2, cx - w / 2, cy + h / 2);
        }

        virtual void DrawCircle(Vector2<T> c, T r, unsigned int segments = 24) {
            DrawCircle(c.x, c.y, r, segments);
        }

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
         * @param poly  the polygon to draw
         * @param scale the scaling factor used to render the normals
         */
        virtual void DrawPolygonNormals(const Polygon<T>& poly, T scale)  {
            if (poly.NumVertices() < 3) { 
                // Degenerated polygon
                return;
            }

            for (size_t i = 0; i < poly.NumEdges(); ++i) {
                const auto& p = poly.GetEdgeCenter(i);
                DrawLine(p, p + poly.GetEdgeNormal(i) * scale);
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

    using ILineRenderer2f = LineRenderer<float>;
    using ILineRenderer2d = LineRenderer<double>;

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

            AddShutdownHook([this](){ lineRenderer = nullptr; });
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

        void SetViewTransform(const Matrix3<T>& m) {
            lineRenderer->SetViewTransform(m);
        }

        void ResetTransform() {
            lineRenderer->ResetTransform();
        }

        void SetTransform(const Matrix3<T>& m) {
            lineRenderer->SetTransform(m);
        }

        const Matrix3<T>& GetTransform() const {
            return lineRenderer->GetTransform();
        }

        void Translate(const Vector2<T>& delta) {
            auto tx = lineRenderer->GetTransform();
            tx.Translate(delta);
            lineRenderer->SetTransform(tx);
        }

        void Rotate(const Vector2<T>& deltaPhi) {
            auto tx = lineRenderer->GetTransform();
            tx.Rotate(deltaPhi);
            lineRenderer->SetTransform(tx);
        }

        void RotateDeg(const Vector2<T>& deltaPhi) {
            auto tx = lineRenderer->GetTransform();
            tx.RotateDeg(deltaPhi);
            lineRenderer->SetTransform(tx);
        }

        void Scale(const Vector2<T>& deltaScale) {
            auto tx = lineRenderer->GetTransform();
            tx.Scale(deltaScale);
            lineRenderer->SetTransform(tx);
        }

        void PushTransform() {
            transformStack.push(lineRenderer->GetTransform());
        }

        void PopTransform() {
            lineRenderer->SetTransform(transformStack.top());
            transformStack.pop();
        }

        void SetDrawColor(const Color<T> & c) {
            lineRenderer->SetDrawColor(c);
        }

        const Color<T>& GetDrawColor() {
            return lineRenderer->GetDrawColor();
        }

        void DrawLine(T x1, T y1, T x2, T y2) {
            lineRenderer->DrawLine(x1, y1, x2, y2);
        }

        void DrawRectangle(T cx, T cy, T w, T h) {
            lineRenderer->DrawRectangle(cx, cy, w, h);
        }

        void DrawCircle(T cx, T cy, T r, unsigned int segments = 24) {
            lineRenderer->DrawCircle(cx, cy, r, segments);
        }

        void DrawCircle(const Vector2<T>& c, T r, unsigned int segments = 24) {
            DrawCircle(c.x, c.y, r, segments);
        }

        void DrawRectangle(const Vector2<T>& c, const Vector2<T>& s) {
            lineRenderer->DrawRectangle(c, s);
        }

        void DrawPolygon(const Polygon<T>& poly) {
            lineRenderer->DrawPolygon(poly);
        }

        void DrawPolygonNormals(const Polygon<T>& poly, T normalScale) {
            lineRenderer->DrawPolygonNormals(poly, normalScale);
        }

        virtual void DrawLine(const Vector2<T> & p1, const Vector2<T> & p2) {
            lineRenderer->DrawLine(p1.x, p1.y, p2.x, p2.y);
        }     

    private:
        /** The line renderer. */
        std::shared_ptr<LineRenderer<T>> lineRenderer;

        /** Used to push and pop transformations. */
        std::stack<Matrix3<T>> transformStack;
    };

    using LineRendererClient2f = LineRendererClient<float>;
    using LineRendererClient2d = LineRendererClient<double>;
    
} // end of namespace