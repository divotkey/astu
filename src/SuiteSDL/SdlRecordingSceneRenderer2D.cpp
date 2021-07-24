/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "SdlRecordingSceneRenderer2D.h"


using namespace std;
using namespace astu::suite2d;

#define ASSERT_VBUF(a) assert(dynamic_cast<SdlVertexBuffer2D*>(&a))
#define VBUF(a) static_cast<const SdlVertexBuffer2D&>(a)

namespace astu {
    
    /////////////////////////////////////////////////
    /////// SdlRecordingSceneRenderer2D
    /////////////////////////////////////////////////

    SdlRecordingSceneRenderer2D::SdlRecordingSceneRenderer2D()
    {
        // Intentionally left empty.
    }

    void SdlRecordingSceneRenderer2D::Render(Polyline& polyline, float alpha)
    {
        SdlSceneRenderer2D::Render(polyline, float alpha);            
    }

} // end of namespace
