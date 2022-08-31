// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

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

    size_t InterpreterExpressionParameterList::NumParameters() {
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

    size_t InterpreterItemParameterList::NumParameters() {
        return parameters.size();
    }

    /////////////////////////////////////////////////
    /////// class InterpreterNoParameterList
    /////////////////////////////////////////////////

    std::shared_ptr<Item> InterpreterNoParameterList::EvaluateParam(ScriptContext &sc, size_t idx) {
        return Item::Create(make_unique<ItemStateUndefined>());
    }

}
