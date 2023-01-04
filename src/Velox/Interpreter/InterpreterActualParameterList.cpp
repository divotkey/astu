/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes.
#include "Velox/Interpreter/InterpreterActualParameterList.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/ItemStateUndefined.h"

// C++ Standard Library includes
#include <cassert>

using namespace std;

namespace velox  {

    /////////////////////////////////////////////////
    /////// class InterpreterExpressionParameterList
    /////////////////////////////////////////////////

    void InterpreterExpressionParameterList::AddParameter(std::shared_ptr<InterpreterExpression> param) {
        parameters.push_back(param);
    }

    std::shared_ptr<Item> InterpreterExpressionParameterList::EvaluateParam(ScriptContext &sc, size_t idx) {
        if (idx < parameters.size()) {
            return parameters[idx]->Evaluate(sc);
        } else {
            return Item::Create(make_unique<ItemStateUndefined>());
        }
    }

    size_t InterpreterExpressionParameterList::NumParameters() const {
        return parameters.size();
    }

    /////////////////////////////////////////////////
    /////// class InterpreterNoParameterList
    /////////////////////////////////////////////////

    void InterpreterItemParameterList::AddParameter(std::shared_ptr<Item> param) {
        parameters.push_back(param);
    }

    std::shared_ptr<Item> InterpreterItemParameterList::EvaluateParam(ScriptContext &sc, size_t idx) {
        assert(idx < parameters.size());
        return parameters[idx];
    }

    size_t InterpreterItemParameterList::NumParameters() const {
        return parameters.size();
    }

    /////////////////////////////////////////////////
    /////// class InterpreterNoParameterList
    /////////////////////////////////////////////////

    std::shared_ptr<Item> InterpreterNoParameterList::EvaluateParam(ScriptContext &sc, size_t idx) {
        return Item::Create(make_unique<ItemStateUndefined>());
    }

}
