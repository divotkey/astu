// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

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


}
