/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "SuiteSDL/SdlSceneRenderer2D.h"

// C++ Standard libraries includes
#include <vector>


// Forward declaration.
struct SDL_Renderer;

namespace astu {

    class SdlRecordingSceneRenderer2D : public SdlSceneRenderer2D {
    public:

        /**
         * Constructor.
         */
        SdlRecordingSceneRenderer2D();

        /**
         * virtual Destructor.
         */
        virtual ~SdlRecordingSceneRenderer2D();

        // Inherited via Scene2Renderer
        virtual void Render(suite2d::Polyline& polyline, float alpha) override;

        virtual void BeginFrame() override;
        virtual void EndFrame() override;

    private:
        struct Line {
            Color4f color;
            Vector2f p0, p1;

            Line(const Color4f& c, const Vector2f& p0, const Vector2f& p1)
                : color(c), p0(p0), p1(p1) {}
        };

        using Frame = std::vector<Line>;
        std::vector<Frame> frames;
        Frame *curFrame;

        void RenderFrames();
        void RenderFrame(const Frame& frame, size_t cnt, const std::string& filename);
    };

} // end of namespace