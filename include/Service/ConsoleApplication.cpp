/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "ConsoleApplication.h"
#include "AstUtils.h"

// C++ Standard Library includes
#include <string>
#include <iostream>

using namespace std;

namespace astu {

    ConsoleApplication::ConsoleApplication()
        : appName("ASTU Console Application")
        , versionString("1.0.0")
    {
        AddCoreServices();
    }

    const std::string& ConsoleApplication::GetVersionString() const
    {
        return versionString;
    }

    void ConsoleApplication::SetVersionString(const string& version)
    {
        versionString = version;
    }

    const std::string& ConsoleApplication::GetApplicationName() const
    {
        return appName;
    }

    void ConsoleApplication::SetApplicationName(const string& name)
    {
        appName = name;
    }

    std::string ConsoleApplication::GetInfoString() const
    {
        return GetApplicationName() + " - Version " + GetVersionString();
    }


    void ConsoleApplication::PrintVersionInfo() {
        cout << GetInfoString() << endl << endl;
        SayVersion();
        SayCopyright(true);
    }

    void ConsoleApplication::AddCoreServices() {

    }

    int ConsoleApplication::Run() {
        PrintVersionInfo();
        return 0;
    }

    void ConsoleApplication::ConfigureApplication() {

    }

    void ConsoleApplication::Cleanup() {

    }

} // end of namespace