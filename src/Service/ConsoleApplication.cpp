// Copyright (c) 2020-2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Service/ConsoleApplication.h"
#include "Service/UpdateService.h"
#include "Service/TaskService.h"
#include "Service/StateService.h"
#include "Service/ConsoleTimeService.h"
#include "Util/SpinLockSleep.h"
#include "Util/StdSleep.h"
#include "Util/VersionInfo.h"
#include "Util/Timer.h"
#include "DeepSleep.h"

// C++ Standard Library includes
#include <string>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

#define DEFAULT_UPS 60.0

namespace astu {

    const string ConsoleApplication::APP_NAME_PROP          = "APP_NAME";
    const string ConsoleApplication::APP_VERSION_PROP       = "APP_VERSION";
    const string ConsoleApplication::COPYRIGHT_YEAR_PROP    = "COPYRIGHT_YEAR";
    const string ConsoleApplication::COPYRIGHT_HOLDER_PROP  = "COPYRIGHT_HOLDER";
    const string ConsoleApplication::SHOW_APP_INFO_PROP     = "SHOW_APP_INFO";

    ConsoleApplication::ConsoleApplication() : terminated(true)
    {
        // Configure predefined flags and properties.
        SetFlag(SHOW_APP_INFO_PROP);
        SetStringProperty(APP_NAME_PROP, "ASTU Console Application");
        SetStringProperty(APP_VERSION_PROP, "1.0.0");
        SetUpdatesPerSecond(DEFAULT_UPS);

        // Add core services required for all service-based applications.
        AddCoreServices();

        // Set default loop mode
        SetLoopMode(LoopMode::OSI_SLEEP);
    }

    void ConsoleApplication::SetUpdatesPerSecond(double ups) {
        targetDelay = nanoseconds(static_cast<long long>(1000000000 / ups));
    }

    double ConsoleApplication::GetUpdatesPerSecond() const {
        return 1000000000.0 / targetDelay.count();
    }

    std::string ConsoleApplication::GetInfoString() const
    {
        return GetApplicationName() + " - Version " + GetVersionString();
    }

    void ConsoleApplication::PrintVersionInfo() {
        cout << GetInfoString() << endl;
        if (HasStringProperty(COPYRIGHT_HOLDER_PROP)) {
            cout << "Copyright ";
            if (HasStringProperty(COPYRIGHT_YEAR_PROP)) {
                cout << GetCopyrightYear() << " ";
            }
            cout << GetCopyrightHolder() << ". All rights reserved." << endl;
        }

        if (IsFlagSet("SHOW_ASTU_VERSION", false)) {
            cout << endl;
            cout << GetVersionInfo << endl;
            if (IsFlagSet("SHOW_ASTU_COPYRIGHT", true)) {
                cout << GetCopyrightInfo() << endl;
            }
        }

        cout << endl;
    }

    void ConsoleApplication::AddCoreServices() {

        // Measures the elapsed time using functions from the Standard C++ Library.
        ASTU_CREATE_AND_ADD_SERVICE( ConsoleTimeService );

        // The update service functions as the central facility for things
        // (services, systems, etc.) that need to be updated once within the
        // main application loop.
        ASTU_CREATE_AND_ADD_SERVICE( UpdateService );

        // Manages different application states.
        ASTU_CREATE_AND_ADD_SERVICE( StateService );

        // Manages different application states.
        ASTU_CREATE_AND_ADD_SERVICE( TaskService );

        // Sends string signals, e.g, used to switch game states.
        ASTU_CREATE_AND_ADD_SERVICE( SignalService<string> );
    }

    int ConsoleApplication::Run() {

        try {
            if (IsFlagSet(SHOW_APP_INFO_PROP)) {
                PrintVersionInfo();
            }

            ASTU_SERVICE(SignalService<string>).AddListener(*this);
            ConfigureApplication();

            ASTU_STARTUP_SERVICES();

            terminated = false;
            RunLoop();

            ASTU_SHUTDOWN_SERVICES();
            ASTU_SERVICE(SignalService<string>).RemoveListener(*this);
        }
        catch (const runtime_error & e) {
            cerr << "Runtime Error: " << e.what() << endl;
            return -1;
        }
        catch (const logic_error & e) {
            cerr << "Internal Application Error: " << e.what() << endl;
            return -1;
        }

        return 0;
    }

    void ConsoleApplication::ConfigureApplication() {
        // not implemented yet
    }

    void ConsoleApplication::Cleanup() {
        // not implemented yet
    }

    /////////////////////////////////////////////////
    /////// Method related to property management
    /////////////////////////////////////////////////

    const std::string& ConsoleApplication::GetStringProperty(const string &name) const {
        auto it = stringProperties.find(name);
        if (it == stringProperties.end()) {
            throw std::logic_error("Unknown string property '" + name + "'");
        }
        return it->second;
    }

    const std::string&  ConsoleApplication::GetStringProperty(const string &name, const string &defaultValue) const {
        auto it = stringProperties.find(name);
        if (it == stringProperties.end()) {
            return defaultValue;
        }
        return it->second;
    }

    bool ConsoleApplication::HasStringProperty(const string &name) const {
        return stringProperties.find(name) != stringProperties.end();
    }

    void ConsoleApplication::SetStringProperty(const string &name, const string &value) {
        stringProperties[name] = value;
    }

    bool ConsoleApplication::IsFlagSet(const string &name, bool defaultValue) const {
        auto it = boolProperties.find(name);
        if (it == boolProperties.end()) {
            return defaultValue;
        }
        return it->second;
    }

    bool ConsoleApplication::IsFlagSet(const string &name) const {
        auto it = boolProperties.find(name);
        if (it == boolProperties.end()) {
            throw std::logic_error("Unknown flag '" + name + "'");
        }
        return it->second;
    }

    void ConsoleApplication::SetFlag(const string &name, bool value) {
        boolProperties[name] = value;
    }

    bool ConsoleApplication::HasFlag(const string &name) const {
        return boolProperties.find(name) != boolProperties.end();
    }

    /////////////////////////////////////////////////
    /////// Method related to application loop
    /////////////////////////////////////////////////

    void ConsoleApplication::RunLoop() {

        auto & updater = ASTU_SERVICE(UpdateService);
        auto & timeSrv = ASTU_SERVICE(TimeService);

        auto prevClock = high_resolution_clock::now();
        while (!terminated) {
            auto nextClock = high_resolution_clock::now();
            auto deltaTime = (nextClock - prevClock).count() / 1e9;
            updater.UpdateAll();
            auto cycleClock = high_resolution_clock::now();

            auto elapsed = chrono::duration_cast<chrono::nanoseconds>(cycleClock - nextClock);
            if (elapsed < targetDelay) {
                //cout << "sleeping " << (targetDelay - elapsed).count() << " ("<< (targetDelay - elapsed).count() / 1e9 * 1000 << " ms)" << endl;
                sleeper->Sleep(targetDelay - elapsed);
            } else {
                cout << "skipped" << endl;
            }
            prevClock = nextClock;

            // Note: deltaTime is currently not used, but might be useful to set the delta time for the
            // time service instead of measuring the delta time within the time service.
            //cout << "dt = " << deltaTime << endl;
        }
    }

    void ConsoleApplication::Terminate() {
        terminated = true;
    }

    bool ConsoleApplication::OnSignal(const string &signal) {
        if (signal == "QUIT" || signal == "TERMINATE") {
            Terminate();
        }

        return true;
    }

    void ConsoleApplication::SetLoopMode(ConsoleApplication::LoopMode mode) {
        switch (mode) {
            case LoopMode::SPIN_LOCK:
                loopMode = LoopMode::SPIN_LOCK;
                sleeper = make_unique<SpinLockSleep>();
                break;

            case LoopMode::OS_SLEEP:
                loopMode = LoopMode::OS_SLEEP;
                sleeper = make_unique<DeepSleep>();
                break;

            case LoopMode::OSI_SLEEP:
                loopMode = LoopMode::OSI_SLEEP;
                sleeper = make_unique<StdSleep>();
                break;
        }
    }

    ConsoleApplication::LoopMode ConsoleApplication::GetLoopMode() const {
        return ConsoleApplication::LoopMode::OS_SLEEP;
    }

} // end of namespace