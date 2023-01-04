/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "SuiteSDL/SdlLineRenderer.h"

// Simple Direct Layer (SDL) includes
#include <SDL2/SDL.h>

// C++ Standard Library includes
#include <cassert>     

namespace astu {

    SdlLineRenderer::SdlLineRenderer(int renderPriority)
        : Service("SDL Line Renderer")
        , SdlRenderLayer(renderPriority)
    {
        // Intentionally left empty.
    }

    void SdlLineRenderer::OnRender(SDL_Renderer* renderer)
    {
        for (auto const & cmd : commands) {
            switch (cmd.type) {
            case CommandType::DRAW_LINE:
                SDL_RenderDrawLine(renderer, cmd.line.x1, cmd.line.y1, cmd.line.x2, cmd.line.y2);
                break;

            case CommandType::SET_COLOR:
                SDL_SetRenderDrawColor(
                    renderer,
                    static_cast<uint8_t>(cmd.color.r), 
                    static_cast<uint8_t>(cmd.color.g), 
                    static_cast<uint8_t>(cmd.color.b), 
                    static_cast<uint8_t>(cmd.color.a)
                    );
                break;
            }
        }
        commands.clear();
    }

    void SdlLineRenderer::DrawLine(float x1, float y1, float x2, float y2)
    {
        RenderCommand cmd;

        const auto& tx = GetModelViewMatrix();
        Vector2f p1 = tx.TransformPoint(x1, y1);
        Vector2f p2 = tx.TransformPoint(x2, y2);


        cmd.type = CommandType::DRAW_LINE;
        cmd.line.x1 = static_cast<int>(p1.x + 0.5f);
        cmd.line.y1 = static_cast<int>(p1.y + 0.5f);
        cmd.line.x2 = static_cast<int>(p2.x + 0.5f);
        cmd.line.y2 = static_cast<int>(p2.y + 0.5f);

        commands.push_back(cmd);
    }

    void SdlLineRenderer::OnSetDrawColor(const Color4f & c) 
    {
        assert(c.r >= 0 && c.r <= 1);
        assert(c.g >= 0 && c.g <= 1);
        assert(c.b >= 0 && c.b <= 1);
        assert(c.a >= 0 && c.a <= 1);

        RenderCommand cmd;
        cmd.type = CommandType::SET_COLOR;
        cmd.color.r = static_cast<int>(c.r * 255);
        cmd.color.g = static_cast<int>(c.g * 255);
        cmd.color.b = static_cast<int>(c.b * 255);
        cmd.color.a = static_cast<int>(c.a * 255);

        commands.push_back(cmd);
    }

    void SdlLineRenderer::OnStartup()
    {
        // Intentionally left empty.
    }

    void SdlLineRenderer::OnShutdown()
    {
        commands.clear();
        commands.resize(0);
        commands.shrink_to_fit();
    }

} // end of namespace
