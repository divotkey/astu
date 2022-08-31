// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterExpression.h"
#include "Velox/Interpreter/InterpreterConstructorCall.h"

// C++ Standard Library includes
#include <string>

namespace velox {

    class InterpreterExpressionNew : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterExpressionNew(unsigned int lineNumber) : InterpreterExpression(lineNumber) {}

        void SetTypeName(const std::string& name);
        void SetConstructorCall(std::shared_ptr<InterpreterConstructorCall> call);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        /** The name of the type of the object which should be created. */
        std::string typeName;

        /** The constructor call of the object to be created. */
        std::shared_ptr<InterpreterConstructorCall> constructorCall;
    };

}



