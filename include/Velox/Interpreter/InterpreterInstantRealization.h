// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterExpression.h"

// C++ Standard Library includes
#include <vector>
#include <memory>

namespace velox {

    class  InterpreterInstantRealization : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterInstantRealization(unsigned int lineNumber);

        void SetTypeName(const std::string& name);
        void AddStatement(std::shared_ptr<InterpreterStatement> statement);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        /** The name of the type of the object which should be created. */
        std::string typeName;

        /** The statements this statement block contains. */
        std::vector<std::shared_ptr<InterpreterStatement>> statements;
    };

}



