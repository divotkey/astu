// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterStatement.h"
#include "Velox/Interpreter/InterpreterFunctionDefinition.h"

// C++ Standard Library includes
#include <string>
#include <vector>
#include <memory>

namespace velox {

    class InterpreterClassDefinition : public InterpreterStatement {
    public:

        /**
         * Constructor.
         *
         * @param name          the name of the class
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterClassDefinition(const std::string & name, unsigned int lineNumber)
            : InterpreterStatement(lineNumber), typeName(name) {}

        void AddFunction(std::shared_ptr<InterpreterFunctionDefinition> function);
        bool HasFunction(const std::string& name) const;

        void Execute(ScriptContext &sc) override;

        void Prepare(ScriptContext &sc) override;

    private:
        std::string typeName;
        std::vector<std::shared_ptr<InterpreterFunctionDefinition>> functions;
    };

}


