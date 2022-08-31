// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Velox/Interpreter/InterpreterExpressionNew.h"
#include "Velox/Interpreter/InterpreterError.h"
#include "Velox/Interpreter/ObjectType.h"
#include "Velox/Interpreter/Item.h"

// C++ Standard Library includes
#include <cassert>

namespace velox {

    std::shared_ptr<Item> InterpreterExpressionNew::Evaluate(ScriptContext &sc) {
        auto type = sc.FindObjectType(typeName);
        if (!type) {
            throw InterpreterError("Unknown object type '" + typeName + "'", GetLineNumber());
        }

        auto result = type->CreateObject(sc);
        assert(constructorCall);
        constructorCall->Execute(sc, result->GetItem(typeName));

        return result;
    }

    void InterpreterExpressionNew::SetConstructorCall(std::shared_ptr<InterpreterConstructorCall> call) {
        constructorCall = call;
    }

    void InterpreterExpressionNew::SetTypeName(const std::string &name) {
        typeName = name;
    }

}