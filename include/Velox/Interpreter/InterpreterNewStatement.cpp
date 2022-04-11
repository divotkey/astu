// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "InterpreterNewStatement.h"
#include "InterpreterError.h"
#include "ObjectType.h"
#include "Item.h"

// C++ Standard Library includes
#include <cassert>

namespace velox {

    std::shared_ptr<Item> InterpreterNewStatement::Evaluate(ScriptContext &sc) {
        auto type = sc.FindObjectType(typeName);
        if (!type) {
            throw InterpreterError("Unknown object type '" + typeName + "'", GetLineNumber());
        }

        auto result = type->CreateObject(sc);
        assert(constructorCall);
        constructorCall->Execute(sc, result->GetItem(typeName));

        return result;
    }

    void InterpreterNewStatement::SetConstructorCall(std::shared_ptr<InterpreterConstructorCall> call) {
        constructorCall = call;
    }

    void InterpreterNewStatement::SetTypeName(const std::string &name) {
        typeName = name;
    }

}