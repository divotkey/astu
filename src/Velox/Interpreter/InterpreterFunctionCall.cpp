// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "Velox/Interpreter/InterpreterFunctionCall.h"
#include "Velox/Interpreter/Item.h"

using namespace std;

namespace velox {

    void InterpreterFunctionCall::SetFunction(std::shared_ptr<InterpreterExpression> inFunction) {
        function = inFunction;
    }

    void InterpreterFunctionCall::AddParameter(std::shared_ptr<InterpreterExpression> param) {
        // Parameters should not be handled as l-values even if it's a variable and hence a location.
        param->SetLocation(false);
        parameters.AddParameter(param);
    }

    std::shared_ptr<Item> InterpreterFunctionCall::Evaluate(ScriptContext &sc) {
        // Create block scope to have a home for temporary items.
        sc.PushCodeBlockScope();
        auto funcItem= function->Evaluate(sc);
        auto result = funcItem->CallAsFunction(sc, parameters, GetLineNumber());
        sc.PopLocalScope();
        return result;
    }

}