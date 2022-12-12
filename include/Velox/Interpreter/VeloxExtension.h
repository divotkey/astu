// DencitiesLab
// Copyright (c) 2020-2022. Roman Divotkey. All rights reserved.
// See LICENSE.TXT file for more information.

#pragma once

// C++ Standard Library includes
#include <string>

namespace velox {

    // Forward declaration.
    class Interpreter;

    class VeloxExtension {
    public:

        VeloxExtension(const std::string &name) : name(name) {}

        /** Virtual destructor. */
        virtual ~VeloxExtension() {}

        virtual void InjectExtension(velox::Interpreter & interpreter) const = 0;

        const std::string& GetName() const {
            return name;
        }

    private:
        const std::string name;
    };

} // end of namespace.