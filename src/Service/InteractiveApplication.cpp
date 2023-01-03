/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Service/InteractiveApplication.h"
#include "Logging/ConsoleLoggingService.h"
#include "Graphics/RalColors.h"
#include "Service/UpdateService.h"
#include "Service/TaskService.h"
#include "Service/WindowService.h"
#include "Service/RenderService.h"
#include "Service/StateService.h"
#include "Service/ThreadService.h"
#include "Service/BlackboardServiceTs.h"
#include "Input/InputSignals.h"
#include "Input/InputMappingService.h"
#include "Input/Keyboard.h"
#include "Util/MessageBox.h"
#include "Util/VersionInfo.h"

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
        , fullscreen(false)
        , fullScreenKey(Keyboard::Keycodes::KEY_F11)
        , vsync(true)
        , resizable(false)
        , running(false)
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

    void InteractiveApplication::SetWindowTitle(const std::string & title)
    {
        windowTitle = title;
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

    void InteractiveApplication::SetResolution(Resolution res)
    {
        startupResolution = res;
    }

    Resolution InteractiveApplication::GetResolution() const
    {
        return startupResolution;
    }

    void InteractiveApplication::SetResizable(bool inResizable)
    {
        resizable = inResizable;
    }

    bool InteractiveApplication::IsResizable() const
    {
        return resizable;
    }

    void InteractiveApplication::SetFullscreen(bool startFullscreen)
    {
        fullscreen = startFullscreen;
    }

    bool InteractiveApplication::IsFullscreen() const
    {
        return fullscreen;
    }

    void InteractiveApplication::SetVSync(bool enableVsync)
    {
        vsync = enableVsync;
    }

    bool InteractiveApplication::IsVsync() const
    {
        return vsync;
    }

    void InteractiveApplication::SetFullscreenToggleKey(int keycode)
    {
        fullScreenKey = keycode;
    }

    int InteractiveApplication::GetFullscreenToggleKey() const
    {
        return fullScreenKey;
    }

    void InteractiveApplication::SetBackgroundColor(const Color4f& color)
    {
        backgroundColor = color;
    }

    const Color4f& InteractiveApplication::GetBackgroundColor() const
    {
        return backgroundColor;
    }

    void InteractiveApplication::SetPrintApplicationInfo(bool printInfo)
    {
        printVersionInfo = printInfo;
    }

    bool InteractiveApplication::IsPrintApplicationInfo() const
    {
        return printVersionInfo;
    }

    void InteractiveApplication::AddCoreServices()
    {
        // Add logging service. (Needs to be revised so that it can be refined).
        ASTU_CREATE_AND_ADD_SERVICE( ConsoleLoggingService );

        // The update service functions as the central facility for things 
        // (services, systems, etc.) that need to be updated once within the
        // main application loop.
        ASTU_CREATE_AND_ADD_SERVICE( UpdateService );

        // Manages different application states.
        ASTU_CREATE_AND_ADD_SERVICE( StateService );

        // Manages different application states.
        ASTU_CREATE_AND_ADD_SERVICE( TaskService );

        // Receives and distributes mouse button signals.
        ASTU_CREATE_AND_ADD_SERVICE( MouseButtonSignalService, "Signal service for mouse button signals" );

        // Receives and distributes mouse wheel signals.
        ASTU_CREATE_AND_ADD_SERVICE( MouseWheelSignalService, "Signal service for mouse wheel signals" );

        // Receives and distributes mouse move signals.
        ASTU_CREATE_AND_ADD_SERVICE( MouseMoveSignalService, "Signal service for mouse move signals" );

        // Receives and distributes keystroke signals.
        ASTU_CREATE_AND_ADD_SERVICE( KeystrokeSignalService, "Signal service for keystroke signals" );

        // Receives and distributes resize signals.
        ASTU_CREATE_AND_ADD_SERVICE( ResizeSignalService, "Signal service for resize signals" );

       // Receives and distributes window state signals.
        ASTU_CREATE_AND_ADD_SERVICE( WindowStateSignalService, "Signal service for WindowState signals" );

        // Maps game actions and input axis.
        ASTU_CREATE_AND_ADD_SERVICE( InputMappingService );

        // Sends string signals, e.g, used to switch game states.
        ASTU_CREATE_AND_ADD_SERVICE( SignalService<string>, "Signal Service for state switches" );

        // Sends string signals, e.g, used to switch game states.
        ASTU_CREATE_AND_ADD_SERVICE( SignalService<string>, "Signal Service for state switches" );

        // Sends string signals, e.g, used to switch game states.
        ASTU_CREATE_AND_ADD_SERVICE( SignalService<string>, "Signal Service for state switches" );

        // Add services related to multi-threading.
        ASTU_CREATE_AND_ADD_SERVICE( ThreadService );
        ASTU_CREATE_AND_ADD_SERVICE( SignalServiceTs<ThreadStatusSignal>, "Signal Service thread status updates" );
        ASTU_CREATE_AND_ADD_SERVICE( BlackboardServiceTs );
    }

    int InteractiveApplication::Run()
    {
        try {
            ConfigureApplication();

            if (printVersionInfo) {
                PrintVersionInfo();
            }

            ASTU_STARTUP_SERVICES();

            auto & updater = ASTU_SERVICE(UpdateService);
            running = true;
            while ( !terminated )
            {
                updater.UpdateAll();
            }
            running = false;

            ASTU_SHUTDOWN_SERVICES();
        }
        catch (const runtime_error & e) {
            cerr << e.what() << endl;
            MessageBox::ShowErrorMessage(e.what(), GetApplicationName() 
                + ": Runtime Error");
            return -1;
        }
        catch (const logic_error & e) {
            cerr << "programmer error: " << e.what() << endl;
            MessageBox::ShowErrorMessage(e.what(), GetApplicationName() 
                + ": Internal Error");
            return -1;
        }

        return 0;
    }

    // Configures services according to application settings and configuration.
    void InteractiveApplication::ConfigureApplication()
    {
        // Configure application main window.
        auto & wndSrv = ASTU_SERVICE(WindowService);
        if (windowTitle.empty()) {
            wndSrv.SetTitle( GetInfoString() );
        } else {
            wndSrv.SetTitle( windowTitle );
        }

        wndSrv.SetSize(startupResolution);
        wndSrv.SetFullscreen(fullscreen);
        wndSrv.SetResizable(resizable);

        // Configure background color and vertical synchronization.
        auto& rndSrv = ASTU_SERVICE(RenderService);
        rndSrv.SetVSync(vsync);
        rndSrv.SetBackgroundColor(backgroundColor);

        ASTU_SERVICE(WindowStateSignalService).AddListener(*this);
        ASTU_SERVICE(KeystrokeSignalService).AddListener(*this);
    }

    void InteractiveApplication::Cleanup()
    {
        ASTU_SERVICE(KeystrokeSignalService).RemoveListener(*this);
        ASTU_SERVICE(WindowStateSignalService).RemoveListener(*this);
    }

    void InteractiveApplication::PrintVersionInfo()
    {
        cout << GetInfoString() << endl << endl;
        cout << GetVersionInfo() << endl;
        cout << GetCopyrightInfo() << endl << endl;
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

    bool InteractiveApplication::OnSignal(const KeystrokeSignal& signal)
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