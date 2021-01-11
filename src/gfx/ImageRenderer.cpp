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

#include "gfx/PatternRenderer.h"
#include "gfx/Quadtree.h"
#include "gfx/Pattern.h"
#include "Vector2.h"
#include "MathUtils.h"
#include "ImageRenderer.h"


namespace astu {

    ImageRenderer::ImageRenderer()
        : root(std::make_unique<UnionPattern>())
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
            renderer = std::make_unique<SimplePatternRenderer>();
            break;

        case RenderQuality::Simple:
            renderer = std::make_unique<AntiAlisaingPatternRenderer>(AntialiasingLevel::Simple);
            break;

        case RenderQuality::Good:
            renderer = std::make_unique<AntiAlisaingPatternRenderer>(AntialiasingLevel::Good);
            break;

        case RenderQuality::Beautiful:
            renderer = std::make_unique<AntiAlisaingPatternRenderer>(AntialiasingLevel::Beautiful);
            break;

        case RenderQuality::Insane:
            renderer = std::make_unique<AntiAlisaingPatternRenderer>(AntialiasingLevel::Insane);
            break;

        // case Quality::VeryGood:
        //     renderer = std::make_unique<StochasticAntiAlisaingPatternRenderer>();
        //     break;

        // case Quality::Insane:
        //     renderer = std::make_unique<StochasticAntiAlisaingPatternRenderer>();
        //     break;
        }
    }

    void ImageRenderer::SetDrawColor(const Color & c)
    {
        drawColor = c;
    }

    void ImageRenderer::SetBackgroundColor(const Color & c)
    {
        backgroundColor = c;
        if (background) {
            background->SetColor(c);
        }
    }

    void ImageRenderer::Clear()
    {
        root->Clear();
        root->Add(background = std::make_shared<UnicolorPattern>(backgroundColor));
        root->Add(quadtree = std::make_shared<Quadtree>(5, 10));
    }

    void ImageRenderer::DrawCircle(double x, double y, double r)
    {
        auto circle = std::make_shared<CirclePattern>(r);
        circle->Translate(x, y);
        circle->SetPattern(std::make_shared<UnicolorPattern>(drawColor));
        quadtree->Add(circle);
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
        quadtree->Add(rect);
    }

    void ImageRenderer::DrawRectangle(double cx, double cy, double w, double h, double phi)
    {
        auto rect = std::make_shared<RectanglePattern>(w, h);
        rect->Translate(cx, cy);
        rect->Rotate(ToRadians(phi));
        rect->SetPattern(std::make_shared<UnicolorPattern>(drawColor));
        quadtree->Add(rect);
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