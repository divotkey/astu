/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

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



