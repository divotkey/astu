/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */


// Local includes
#include "Graphics/ImageRenderer.h"
#include "Graphics/SimplePatternRenderer.h"
#include "Graphics/AntiAliasingPatternRenderer.h"
#include "Graphics/QuadtreePattern.h"
#include "Graphics/Pattern.h"
#include "Graphics/UnionPattern.h"
#include "Graphics/UnicolorPattern.h"
#include "Graphics/RectanglePattern.h"
#include "Graphics/CirclePattern.h"
#include "Graphics/WebColors.h"
#include "Math/Vector2.h"
#include "Math/MathUtils.h"

// C++ Standard Library includes
#include <stdexcept>
#include <string>

namespace astu {

    ImageRenderer::ImageRenderer(unsigned int maxDepth)
        : root(std::make_unique<UnionPattern>())
        , quadtreeDepth(maxDepth)
    {
        SetRenderQuality(RenderQuality::Good);
        SetDrawColor(WebColors::Black);
        SetBackgroundColor(WebColors::White);
        Clear();
    }


    ImageRenderer::~ImageRenderer()
    {
        // Intentionally left empty.
    }

    void ImageRenderer::SetRenderQuality(RenderQuality quality)
    {
        if (this->quality == quality && renderer != nullptr) {
            return;
        }
        this->quality = quality;

        switch (quality) {

        case RenderQuality::Fast:
            renderer = std::make_unique<SimplePatternRenderer>(PatternRenderer::TransformMode::RAW);
            break;

        case RenderQuality::Simple:
            renderer = std::make_unique<AntiAliasingPatternRenderer>(PatternRenderer::TransformMode::RAW, AntialiasingLevel::Simple);
            break;

        case RenderQuality::Good:
            renderer = std::make_unique<AntiAliasingPatternRenderer>(PatternRenderer::TransformMode::RAW, AntialiasingLevel::Good);
            break;

        case RenderQuality::Beautiful:
            renderer = std::make_unique<AntiAliasingPatternRenderer>(PatternRenderer::TransformMode::RAW, AntialiasingLevel::Beautiful);
            break;

        case RenderQuality::Insane:
            renderer = std::make_unique<AntiAliasingPatternRenderer>(PatternRenderer::TransformMode::RAW, AntialiasingLevel::Insane);
            break;
        }
    }

    void ImageRenderer::SetDrawColor(const Color4d & c) noexcept
    {
        drawColor = c;
    }

    const Color4d & ImageRenderer::GetDrawColor() const noexcept
    {
        return drawColor;
    }

    void ImageRenderer::SetBackgroundColor(const Color4d & c) noexcept
    {
        backgroundColor = c;
        if (background) {
            background->SetColor(c);
        }
    }

    const Color4d & ImageRenderer::GetBackgroundColor() const noexcept
    {
        return backgroundColor;
    }

    void ImageRenderer::Clear()
    {
        root->Clear();
        root->AddPattern(background = std::make_shared<UnicolorPattern>(backgroundColor));
        root->AddPattern(quadtree = std::make_shared<QuadtreePattern>(5, static_cast<int>(quadtreeDepth)));
    }

    void ImageRenderer::DrawCircle(double x, double y, double r)
    {
        auto circle = std::make_shared<CirclePattern>(r);
        circle->Translate(x, y);
        circle->SetPattern(std::make_shared<UnicolorPattern>(drawColor));
        quadtree->AddPattern(circle);
    }

    void ImageRenderer::DrawLine(double x0, double y0, double x1, double y1, double w)
    {
        Vector2<double> v(x1 - x0, y1 - y0);

        double lng = v.Length();
        if (lng <= 0) {
            return;
        }

        double a = v.Angle(Vector2<double>(1, 0));

        auto rect = std::make_shared<RectanglePattern>(lng, w);
        rect->Translate((x1 + x0) / 2, (y1 + y0) / 2);
        rect->Rotate(-a);
        rect->SetPattern(std::make_shared<UnicolorPattern>(drawColor));
        quadtree->AddPattern(rect);
    }

    void ImageRenderer::DrawRectangle(double cx, double cy, double w, double h, double phi)
    {
        auto rect = std::make_shared<RectanglePattern>(w, h);
        rect->Translate(cx, cy);
        rect->Rotate(ToRadians(phi));
        rect->SetPattern(std::make_shared<UnicolorPattern>(drawColor));
        quadtree->AddPattern(rect);
    }

    void ImageRenderer::SetQuadtreeDepth(unsigned int depth)
    {
        if (depth < 1) {
            throw std::domain_error(std::string("Invalid depth for scene quadtree ") 
                + std::to_string(depth));
        }
        quadtreeDepth = depth;
    }


    void ImageRenderer::Render(Image & img)
    {
        assert(renderer);
        assert(quadtree);

        if (!quadtree->IsEmpty()) {
            quadtree->BuildTree();
        }
        renderer->Render(*root, img);
    }

} // end of namespace