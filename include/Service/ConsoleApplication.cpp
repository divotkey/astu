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
#include <stdexcept>

using namespace std;

namespace astu {

    const string ConsoleApplication::APP_NAME_PROP = "APP_NAME";
    const string ConsoleApplication::APP_VERSION_PROP = "APP_VERSION";
    const string ConsoleApplication::COPYRIGHT_YEAR_PROP = "COPYRIGHT_YEAR";
    const string ConsoleApplication::COPYRIGHT_HOLDER_PROP = "COPYRIGHT_HOLDER";

    ConsoleApplication::ConsoleApplication()
    {
        SetFlag("PRINT_COPYRIGHT");
        SetFlag("PRINT_ASTU_INFO");
        SetStringProperty(APP_NAME_PROP, "ASTU Console Application");
        SetStringProperty(APP_VERSION_PROP, "1.0.0");
        AddCoreServices();
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
            SayVersion();
            if (IsFlagSet("SHOW_ASTU_COPYRIGHT", true)) {
                SayCopyright();
            }
        }

        cout << endl;
    }

    void ConsoleApplication::AddCoreServices() {
        // not implemented yet
    }

    int ConsoleApplication::Run() {
        PrintVersionInfo();
        return 0;
    }

    void ConsoleApplication::ConfigureApplication() {
        // not implemented yet
    }

    void ConsoleApplication::Cleanup() {
        // not implemented yet
    }

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

} // end of namespace