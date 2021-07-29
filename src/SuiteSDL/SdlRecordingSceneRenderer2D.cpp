/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "SdlRecordingSceneRenderer2D.h"
#include "AstuGraphics.h"
#include "AstuMath.h"
#include "AstuServices.h"

// C++ Standard Library includes
#include <sstream>
#include <iomanip>

using namespace std;
using namespace astu::suite2d;

#define ASSERT_VBUF(a) assert(dynamic_cast<SdlVertexBuffer2D*>(&a))
#define VBUF(a) static_cast<const SdlVertexBuffer2D&>(a)
#define TO_COLOR4D(c) Color4d(c.r, c.g, c.b, c.a)
#define TO_VEC2D(v) Vector2d(v.x, v.y)

namespace astu {
    
    /////////////////////////////////////////////////
    /////// SdlRecordingSceneRenderer2D
    /////////////////////////////////////////////////

    SdlRecordingSceneRenderer2D::SdlRecordingSceneRenderer2D()
        : curFrame(nullptr)
    {
        // Intentionally left empty.
    }

    SdlRecordingSceneRenderer2D::~SdlRecordingSceneRenderer2D()
    {
        RenderFrames();
    }

    void SdlRecordingSceneRenderer2D::Render(Polyline& polyline, float alpha)
    {
        assert(curFrame);
        SdlSceneRenderer2D::Render(polyline, alpha);            

        const auto & vertices = VBUF(polyline.GetVertexBuffer()).vertices;
        const auto & tx = viewMatrix * polyline.GetWorldMatrix();
        auto it = vertices.cbegin();
        auto p1 = tx.TransformPoint(*it);

        auto c = polyline.GetColor() * alpha;
        while (++it != vertices.cend()) {
            const auto p2 = tx.TransformPoint(*it);
            curFrame->push_back(Line(c, p1, p2));
            p1 = p2;
        }
    }

    void SdlRecordingSceneRenderer2D::BeginFrame()
    {
        frames.push_back(Frame());
        curFrame = &frames.back();      
    }

    void SdlRecordingSceneRenderer2D::EndFrame()
    {
        curFrame = nullptr;
    }

    void SdlRecordingSceneRenderer2D::RenderFrames()
    {
        string filepath = "/media/roman/Volume/Temp/breakout/frame";        
        cout << "Rendering #" << frames.size() << " frames" << endl;

        for (size_t i = 0; i < frames.size(); ++i) {
            std::stringstream ss;
            ss << filepath << std::setw(4) << std::setfill('0') << i << ".bmp";
            RenderFrame(frames[i], i, ss.str());
        }
    }

    void SdlRecordingSceneRenderer2D::RenderFrame(const Frame& frame, size_t cnt, const std::string& filename)
    {
        ImageRenderer imgRndr;

        auto bgCol = TO_COLOR4D(ASTU_SERVICE(RenderService).GetBackgroundColor());
        imgRndr.SetBackgroundColor(bgCol);

        for (const auto& line : frame) {
            imgRndr.SetDrawColor(TO_COLOR4D(line.color));
            imgRndr.DrawLine(TO_VEC2D(line.p0), TO_VEC2D(line.p1), 2);
        }
        
        auto& wndSrv = ASTU_SERVICE(WindowService);
        Image image(wndSrv.GetWidth(), wndSrv.GetHeight());
        imgRndr.SetRenderQuality(RenderQuality::Insane);
        cout << "Rendering frame #" << cnt << " (" << filename << ")" << endl;
        imgRndr.Render(image);

        StoreImage(image, filename);
    }



} // end of namespace