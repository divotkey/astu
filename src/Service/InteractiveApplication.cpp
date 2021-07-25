/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Service/InteractiveApplication.h"
#include "AstUtils.h"
#include "Graphics/RalColors.h"
#include "Service/UpdateService.h"
#include "Service/WindowService.h"
#include "Service/RenderService.h"
#include "Input/InputSignals.h"
#include "Input/InputMappingService.h"
#include "Input/Keyboard.h"


// C++ Standard Library includes
#include <iostream>

using namespace std;

namespace astu {

    InteractiveApplication::InteractiveApplication()
        : terminated(false)
        , versionString("1.0.0")
        , appName("ASTU Application")
        , backgroundColor(RalColors::TrafficBlack)
        , terminateOnClose(true)
        , terminateOnEscape(true)
        , printVersionInfo(true)
        , startupResolution(Resolution::HD_2)
        , fullScreenKey(Keyboard::Keycodes::KEY_F11)
    {
        AddCoreServices();
    }

    const std::string& InteractiveApplication::GetVersionString() const
    {
        return versionString;
    }

    void InteractiveApplication::SetVersionString(const string& version)
    {
        versionString = version;
    }

    const std::string& InteractiveApplication::GetApplicationName() const
    {
        return appName;
    }

    void InteractiveApplication::SetApplicationName(const string& name)
    {
        appName = name;
    }

    std::string InteractiveApplication::GetInfoString() const
    {
        return GetApplicationName() + " - Version " + GetVersionString();
    }

    void InteractiveApplication::ScheduleTermination()
    {
        terminated = true;
    }

    void InteractiveApplication::EnableTerminateOnClose(bool b)
    {
        terminateOnClose = b;
    }

    bool InteractiveApplication::IsTerminateOnCloseEnabled() const
    {
        return terminateOnClose;
    }

    void InteractiveApplication::EnableTerminateOnEscape(bool b)
    {
        terminateOnEscape = b;
    }

    bool InteractiveApplication::IsTerminateOnEscapeEnabled() const
    {
        return terminateOnEscape;
    }

    void InteractiveApplication::SetStartupResolution(Resolution res)
    {
        startupResolution = res;
    }

    Resolution InteractiveApplication::GetStartupResolution() const
    {
        return startupResolution;
    }

    void InteractiveApplication::SetFullscreenToggleKey(int keycode)
    {
        fullScreenKey = keycode;
    }

    int InteractiveApplication::GetFullscreenToggleKey() const
    {
        return fullScreenKey;
    }

    void InteractiveApplication::AddCoreServices()
    {
        // The update service functions as the central facility for things 
        // (services, systems, etc) that need to be updated once within the 
        // main application loop.
        ASTU_CREATE_AND_ADD_SERVICE( UpdateService );

        // Receives and distributes mouse button signals.
        ASTU_CREATE_AND_ADD_SERVICE( MouseButtonEventService );

        // Receives and distributes mouse wheel signals.
        ASTU_CREATE_AND_ADD_SERVICE( MouseWheelEventService );

        // Receives and distributes mouse move signals.
        ASTU_CREATE_AND_ADD_SERVICE( MouseMoveEventService );

        // Receives and distributes keystroke signals.
        ASTU_CREATE_AND_ADD_SERVICE( KeystrokeEventService );

        // Receives and distributes resize signals.
        ASTU_CREATE_AND_ADD_SERVICE( ResizeEventService );

       // Receives and distributes window state signals.
        ASTU_CREATE_AND_ADD_SERVICE( WindowStateService );

        // Mapps game actions and input axis.
        ASTU_CREATE_AND_ADD_SERVICE( InputMappingService );

        // Mapps game actions and input axis.
        ASTU_CREATE_AND_ADD_SERVICE( InputMappingService );        
    }

    void InteractiveApplication::Run()
    {
        ConfigureApplication();

        ASTU_STARTUP_SERVICES();
        if (printVersionInfo) {
            PrintVersionInfo();
        }

        auto & updater = ASTU_SERVICE(UpdateService);
        while ( !terminated )
        {
            updater.UpdateAll();
        }

        ASTU_SHUTDOWN_SERVICES();
    }

    // Configures services according to application settings and configuration.
    void InteractiveApplication::ConfigureApplication()
    {
        // Configure application main window.
        auto & wndSrv = ASTU_SERVICE(WindowService);
        wndSrv.SetTitle( GetInfoString() );
        wndSrv.SetSize(startupResolution);

        // Configure background color
        ASTU_SERVICE(RenderService).SetBackgroundColor(backgroundColor);

        ASTU_SERVICE(WindowStateService).AddListener(*this);
        ASTU_SERVICE(KeystrokeEventService).AddListener(*this);
    }

    void InteractiveApplication::Cleanup()
    {
        ASTU_SERVICE(KeystrokeEventService).RemoveListener(*this);
        ASTU_SERVICE(WindowStateService).RemoveListener(*this);
    }

    void InteractiveApplication::PrintVersionInfo()
    {
        cout << GetInfoString() << endl << endl;
        SayVersion();
        SayCopyright(true);
    }

    bool InteractiveApplication::OnSignal(const WindowState& signal) 
    {
        switch (signal.type) {
        case WindowState::Type::CLOSE:
            if (terminateOnClose) {
                ScheduleTermination();
                return true;
            }
            break;

        default:
            // Ignore.
            break;
        }

        return false;
    }

    bool InteractiveApplication::OnSignal(const KeystrokeEvent& signal)
    {
        if (signal.pressed) {
            return false;
        }

        switch (signal.keycode) {
        case Keyboard::KEY_ESCAPE:
            if (terminateOnEscape) {
                ScheduleTermination();
                return true;
            }
            break;
        
        default:
            if (signal.keycode == fullScreenKey) {
                auto & wndSrv = ASTU_SERVICE(WindowService);
                wndSrv.SetFullscreen(!wndSrv.IsFullscreen());
                return true;
            }
            break;
        }

        return false;
    }

} // end of namespace