/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "AstuSuite2D.h"
#include "SuiteSDL/SdlApplication.h"
#include "SuiteSDL/SdlService.h"
#include "SuiteSDL/SdlVideoService.h"
#include "SuiteSDL/SdlRenderService.h"
#include "SuiteSDL/SdlEventService.h"
#include "SuiteSDL/SdlJoystickService.h"
#include "SuiteSDL/SdlTimeService.h"
#include "SuiteSDL/SdlSceneGraph2D.h"
#include "SuiteSDL/SdlLineRenderer.h"
#include "SuiteSDL/SdlFontFactory.h"

namespace astu {

    SdlApplication::SdlApplication()
        : add2DServices(true)
    {
        AddSdlServices();
    }

    void SdlApplication::AddServices2D(bool b)
    {
        add2DServices = b;
    }

    bool SdlApplication::IsServices2D() const
    {
        return add2DServices;
    }

    void SdlApplication::AddSdlServices()
    {
        // Initializes and de-initializes SDL and its subsystems.
        ASTU_CREATE_AND_ADD_SERVICE( SdlService );

        // Maintains main application window.
        ASTU_CREATE_AND_ADD_SERVICE( SdlVideoService );

        // Offers an layer-based 2D graphics facility based on SDL render mechanism.
        ASTU_CREATE_AND_ADD_SERVICE( SdlRenderService );

        // Enables the use of gamepads etc.
        ASTU_CREATE_AND_ADD_SERVICE( SdlJoystickService );

        // Empties the SDL-Event queue and distributes events.
        ASTU_CREATE_AND_ADD_SERVICE( SdlEventService );

        // Provides an SDL-based implementation of the ITimeService interface.
        ASTU_CREATE_AND_ADD_SERVICE( SdlTimeService );

        // Provides an SDL-based implementation of the FontFactory service.
        ASTU_CREATE_AND_ADD_SERVICE( SdlFontFactory );
    }

    void SdlApplication::AddSdl2DServices()
    {
        //XXX move addition of camera service to base class, maybe by using properties, when available.
        ASTU_CREATE_AND_ADD_SERVICE( suite2d::CameraService );
        ASTU_CREATE_AND_ADD_SERVICE( SdlSceneGraph2D );
        ASTU_CREATE_AND_ADD_SERVICE( SdlLineRenderer );
        ASTU_CREATE_AND_ADD_SERVICE( SdlVertexBufferBuilderService2D );
        ASTU_CREATE_AND_ADD_SERVICE( SdlTextureFactoryService );
    }

    void SdlApplication::RemoveSdl2DServices()
    {    
        ASTU_REMOVE_SERVICE( SdlVertexBufferBuilderService2D );
        ASTU_REMOVE_SERVICE( SdlSceneGraph2D );
        ASTU_REMOVE_SERVICE( SdlLineRenderer );    
    }

    void SdlApplication::ConfigureApplication()
    {
        InteractiveApplication::ConfigureApplication();
        if (add2DServices) {
            AddSdl2DServices();
        }
    }

    void SdlApplication::Cleanup()
    {
        if (add2DServices) {
            RemoveSdl2DServices();
        }
        InteractiveApplication::Cleanup();
    }

} // end of namespace
