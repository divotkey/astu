/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterVector.h"
#include "Velox/Interpreter/Item.h"

using namespace std;
using namespace astu;

namespace velox {

    shared_ptr<Item> InterpreterVector::Evaluate(ScriptContext &sc) {
        if (!z) {
            return Item::CreateVector2(
                    Vector2d(x->Evaluate(sc)->GetRealValue(GetLineNumber()),
                             y->Evaluate(sc)->GetRealValue(GetLineNumber()))
                             );
        } else {
            throw std::logic_error("Vector3 not implemented yet");
        }
    }

    void InterpreterVector::SetXValueExpression(std::shared_ptr<InterpreterExpression> inX) {
        x = inX;
    }

    void InterpreterVector::SetYValueExpression(std::shared_ptr<InterpreterExpression> inY) {
        y = inY;
    }

    void InterpreterVector::SetZValueExpression(std::shared_ptr<InterpreterExpression> inZ) {
        z = inZ;
    }

} // end of namespace
