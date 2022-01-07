/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard libraries includes
#include <vector>
#include <cstdint>

// Local includes
#include "SdlRenderService.h"
#include "Suite2D/LineRenderer.h"

namespace astu {

    /**
     * A SDL-based implementation of the ILineRenderer interface.
     * 
     * This service is a SDL render layer and uses the command design pattern
     * to store the render calls and replays them when the render layer should
     * be rendered.
     * 
     * @ingroup sdl_group 
     */
    class SdlLineRenderer final 
        : public SdlRenderLayer
        , public suite2d::LineRenderer<float>
    {
    public:

        /**
         * Constructor.
         * 
         * @param renderPriority    the priority of this render layer
         */
        SdlLineRenderer(int renderPriority = Priority::Normal);

        /**
         * Virtual destructor.
         */
        virtual ~SdlLineRenderer() {}

        // Inherited via SdlRenderLayer
        virtual void OnRender(SDL_Renderer* renderer) override;

        // Inherited via ILineRenderer2f
        virtual void DrawLine(float x1, float y1, float x2, float X2) override;
        virtual void OnSetDrawColor(const Color4f & c) override;

    protected:

        // Inherited via SdlRenderLayer
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

    private:
        /** Enumeration for types of render commands. */
        enum CommandType {DRAW_LINE, SET_COLOR};

        struct DrawLineCommand {
            CommandType type;
            int x1, y1, x2, y2;
        };

        struct SetColorCommand {
            CommandType type;
            int r, g, b, a;
        };

        /** Union holding data for all types of render commands. */
        union RenderCommand {
            CommandType type;
            DrawLineCommand line;
            SetColorCommand color;
        };

        /** The current render commands to be processed. */
        std::vector<RenderCommand> commands;
    };

} // end of namespace