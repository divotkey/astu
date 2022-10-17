/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "SuiteSDL/SdlSceneGraph2D.h"
#include "SuiteSDL/SdlSceneRenderer2D.h"
#include "SuiteSDL/SdlRecordingSceneRenderer2D.h"
#include "SuiteSDL/SdlTexture.h"
#include "Graphics/Image.h"

// Simple Direct Layer (SDL) includes
#include <SDL2/SDL.h>

// C++ Standard Library includes
#include <cassert>
#include <memory>
#include <string>

namespace astu {

    /////////////////////////////////////////////////
    /////// SdlVertexBuffer2Builder
    /////////////////////////////////////////////////

    SdlVertexBufferBuilderService2D::SdlVertexBufferBuilderService2D()
            : Service("SDL Vertex Buffer 2D Builder Service")
    {
        Reset();
    }

    VertexBufferBuilder2f &SdlVertexBufferBuilderService2D::AddVertex(float x, float y)
    {
        vertices.push_back({x, y});
        return *this;
    }

    const Vector2f &SdlVertexBufferBuilderService2D::GetVertex(size_t idx) const
    {
        return vertices.at(idx);
    }

    VertexBufferBuilder2f &SdlVertexBufferBuilderService2D::SetVertex(size_t idx, float x, float y)
    {
        vertices.at(idx).Set(x, y);
        return *this;
    }

    size_t SdlVertexBufferBuilderService2D::GetNumVertices() const
    {
        return vertices.size();
    }

    VertexBufferBuilder2f &SdlVertexBufferBuilderService2D::Reset()
    {
        vertices.clear();
        return *this;
    }

    std::shared_ptr<VertexBuffer2f> SdlVertexBufferBuilderService2D::Build()
    {
        auto result = std::make_shared<SdlVertexBuffer2D>();
        result->vertices = vertices;
        return result;
    }

    /////////////////////////////////////////////////
    /////// TextureFactory
    /////////////////////////////////////////////////

    SdlTextureFactoryService::SdlTextureFactoryService()
            : Service("SDL Texture Factory Service")
    {
        // Intentionally left empty.
    }

    std::shared_ptr<Texture> SdlTextureFactoryService::CreateFromImage(const Image &image, ScaleQuality sq) const
    {
        // Create surface with appropriate size and properties.
        SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(
                0,
                image.GetWidth(),
                image.GetHeight(),
                32,
                SDL_PixelFormatEnum::SDL_PIXELFORMAT_ABGR32
        );

        if (SDL_SetSurfaceBlendMode(surface, SDL_BlendMode::SDL_BLENDMODE_BLEND)) {
            throw std::runtime_error(std::string("Unable to set blend mode for SDL surface: ") + SDL_GetError());
        }

        // Transfer pixel data.
        char *pixelBytes = reinterpret_cast<char *>(surface->pixels);

        for (int y = 0; y < image.GetHeight(); ++y) {
            for (int x = 0; x < image.GetWidth(); ++x) {
                Color4d col = image.GetPixel(x, y);
                *reinterpret_cast<int32_t *>(pixelBytes + y * surface->pitch +
                                             x * surface->format->BytesPerPixel) = col.ToRgba();
            }
        }

        //SDL_HINT_RENDER_SCALE_QUALITY
        switch (sq) {
            case ScaleQuality::Nearest:
                SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
                break;
            case ScaleQuality::Linear:
                SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
                break;

            case ScaleQuality::Best:
                SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
                break;
        }
        
        // Create texture from surface.
        SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(ASTU_SERVICE(SdlRenderService).GetRenderer(), surface);
        if (!sdlTexture) {
            SDL_FreeSurface(surface);
            throw std::runtime_error(std::string("Unable to create SDL texture: ") + SDL_GetError());
        }
        SDL_FreeSurface(surface);

        // Create and return wrapper class.
        return std::make_shared<SdlTexture>(sdlTexture);
    }

    std::shared_ptr<Texture> SdlTextureFactoryService::CreateFromBmp(const std::string &filepath, ScaleQuality sq) const
    {
        SDL_Surface *surface = SDL_LoadBMP(filepath.c_str());
        if (!surface) {
            throw std::runtime_error("Unable to load .BMP file '" + filepath + "': " + SDL_GetError());
        }

        SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(ASTU_SERVICE(SdlRenderService).GetRenderer(), surface);
        if (!sdlTexture) {
            SDL_FreeSurface(surface);
            throw std::runtime_error(
                    "Unable to create SDL texture using .BMP file '" + filepath + "': " + SDL_GetError());
        }

        SDL_FreeSurface(surface);

        return std::make_shared<SdlTexture>(sdlTexture);
    }

    /////////////////////////////////////////////////
    /////// SdlSceneGraph2
    /////////////////////////////////////////////////

    SdlSceneGraph2D::SdlSceneGraph2D(int renderPriority, int updatePriority)
            : Service("SDL Scene Graph 2D"), Updatable(updatePriority), SdlRenderLayer(renderPriority)
    {
        // Intentionally left empty.
    }

    SdlSceneGraph2D::~SdlSceneGraph2D()
    {
        // Intentionally left empty.
    }

    void SdlSceneGraph2D::OnRender(SDL_Renderer *renderer)
    {
        sceneRenderer->SetViewMatrix(GetCamera().GetMatrix());
        sceneRenderer->SetSdlRenderer(*renderer);
        sceneRenderer->BeginFrame(GetAbsoluteTime());
        GetRoot()->Render(*sceneRenderer, 1.0f);
        sceneRenderer->EndFrame();
        sceneRenderer->ClearSdlRenderer();
    }

    void SdlSceneGraph2D::OnUpdate()
    {
        GetRoot()->Update(GetElapsedTime());
    }

    void SdlSceneGraph2D::OnStartup()
    {
        sceneRenderer = std::make_unique<SdlSceneRenderer2D>();
        // sceneRenderer = std::make_unique<SdlRecordingSceneRenderer2D>();
    }

    void SdlSceneGraph2D::OnShutdown()
    {
        sceneRenderer = nullptr;
    }

} // end of namespace
