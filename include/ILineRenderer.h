/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local (AST-Utilities) includes
#include "Service.h"
#include "Vector2.h"
#include "Matrix3.h"
#include "Color.h"

namespace astu {

    /**
     * Interface for simple line rendering.
     * 
     * @ingroup gfx_group
     */
    template <typename T>
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
        virtual void DrawLine(
            const Vector2<T> & p1,
            const Vector2<T> & p2)
        {
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
        virtual void DrawLine(T x1, T y1, T x2, T y2) = 0;

        /**
         * Sets the current drawing color used for all subsequent drawing calls.
         * 
         * @param c the new drawing color 
         */
        virtual void SetDrawColor(const Color<T> & c) = 0;

        /**
         * Sets the stransformation matrix used to render lines.
         * 
         * @param m the transformation matrix
         */
        virtual void SetTransform(const Matrix3<T> & m) = 0;
    };

    using ILineRenderer2f = ILineRenderer<float>;
    using ILineRenderer2d = ILineRenderer<double>;

    template <typename T>
    class LineRendererClient : public virtual Service {
    public:

        /**
         * Constructor.
         */
        LineRendererClient(){
            AddStartupHook([this](){ 
                lineRenderer = ASTU_GET_SERVICE(ILineRenderer<T>); 
            });

            AddShutdownHook([this](){ lineRenderer = nullptr; });
        }

    protected:

        /**
         * Returns the line renderer.
         * 
         * @return the line renderer
         */
        ILineRenderer<T> & GetLineRenderer() {
            return *lineRenderer;
        }

        void SetDrawColor(const Color<T> & c) {
            lineRenderer->SetDrawColor(c);
        }

        void DrawLine(T x1, T y1, T x2, T y2) {
            lineRenderer->DrawLine(x1, y1, x2, y2);
        }

        void DrawRectangle(T cx, T cy, T w, T h) {
            lineRenderer->DrawLine(cx - w / 2, cy - h / 2, cx + w / 2, cy - h / 2);
            lineRenderer->DrawLine(cx - w / 2, cy - h / 2, cx - w / 2, cy + h / 2);
            lineRenderer->DrawLine(cx + w / 2, cy - h / 2, cx + w / 2, cy + h / 2);
            lineRenderer->DrawLine(cx + w / 2, cy + h / 2, cx - w / 2, cy + h / 2);
        }

        void DrawRectangle(const Vector2<T>& c, const Vector2<T>& s) {
            DrawRectangle(c.x, c.y, s.x, s.y);
        }

        virtual void DrawLine(const Vector2<T> & p1, const Vector2<T> & p2) {
            lineRenderer->DrawLine(p1.x, p1.y, p2.x, p2.y);
        }     



    private:
        /** The line renderer. */
        std::shared_ptr<ILineRenderer<T>> lineRenderer;
    };

    using LineRendererClient2f = LineRendererClient<float>;
    using LineRendererClient2d = LineRendererClient<double>;
} // end of namespace