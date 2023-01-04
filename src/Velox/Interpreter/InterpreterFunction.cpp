/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#include "Velox/Interpreter/InterpreterFunction.h"
#include "Velox/Interpreter/Scope.h"
#include "Velox/Interpreter/InterpreterError.h"
#include "Velox/Interpreter/Item.h"

// C++ Standard Library
#include <cassert>
#include <algorithm>
#include <string>

using namespace std;

namespace velox {

    std::shared_ptr<Item>
    InterpreterFunction::Evaluate(ScriptContext &sc,
                                  InterpreterActualParameterList &actualParameters,
                                  std::shared_ptr<Scope> memberScope,
                                  unsigned int lineNumber)
    {
        // Actual parameters must be less or equal to formal parameters.
        if (formalParameters.size() < actualParameters.NumParameters()) {
            throw InterpreterError(string("function call with to many parameters (expected ") +
                                   std::to_string(formalParameters.size()) + " but got " +
                                   to_string(actualParameters.NumParameters()) + ")", lineNumber);
        }

        // Create a new scope for the function parameters.
        auto parameterScope = make_shared<Scope>();

        // Fill scope with parameters, either actual evaluated params or undefined.
        for (size_t i = 0; i < formalParameters.size(); ++i) {
            parameterScope->AddItem(formalParameters[i], actualParameters.EvaluateParam(sc, i));
        }

        // Script context must contain parameters before actual function is evaluated.
        sc.PushFunctionScope(parameterScope);
        if (memberScope)
            sc.PushCodeBlockScope(memberScope);

        // DoEvaluate/evaluate the actual function.
        auto result = DoEvaluate(sc, lineNumber);

        // Clean up the scope stack.
        if (memberScope)
            sc.PopLocalScope();
        sc.PopLocalScope();

        if (!result) {
            result = Item::CreateUndefined();
        }
        return result;
    }

    void InterpreterFunction::AddFormalParameter(const string &simpleName) {
        assert(!HasFormalParameter(simpleName));
        formalParameters.push_back(simpleName);
    }

    bool InterpreterFunction::HasFormalParameter(const string &simpleName) const {
        return find(formalParameters.begin(), formalParameters.end(), simpleName) != formalParameters.end();
    }

}
