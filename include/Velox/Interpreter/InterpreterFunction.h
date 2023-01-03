/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes.
#include "InterpreterActualParameterList.h"

namespace velox {

    // Forward declaration
    class Item;
    class Scope;

    class InterpreterFunction {
    public:

        /** Virtual destructor. */
        virtual ~InterpreterFunction() {}

        std::shared_ptr<Item>
        Evaluate(ScriptContext &sc, InterpreterActualParameterList &actualParameters, std::shared_ptr<Scope> memberScope, unsigned int lineNumber);
        void AddFormalParameter(const std::string& simpleName);
        bool HasFormalParameter(const std::string& simpleName) const;

    protected:
        virtual std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) = 0;

    private:
        std::vector<std::string> formalParameters;
    };

} // end of namespace


