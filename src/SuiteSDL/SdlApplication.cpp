/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "SuiteSDL/SdlApplication.h"
#include "SuiteSDL/SdlService.h"
#include "SuiteSDL/SdlVideoService.h"
#include "SuiteSDL/SdlRenderService.h"
#include "SuiteSDL/SdlEventService.h"
#include "SuiteSDL/SdlJoystickService.h"
#include "SuiteSDL/SdlTimeService.h"

namespace astu {

    SdlApplication::SdlApplication()
    {
        AddSdlServices();
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
    }

} // end of namespace
