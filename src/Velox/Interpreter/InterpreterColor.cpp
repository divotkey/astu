/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterColor.h"
#include "Velox/Interpreter/Item.h"

using namespace std;
using namespace astu;

namespace velox {

    shared_ptr<Item> InterpreterColor::Evaluate(ScriptContext &sc) {
        if(!green || !blue) {
            return Item::CreateColor(Color4d(red->Evaluate(sc)->GetIntegerValue()));
        } else {
            double r = red->Evaluate(sc)->GetRealValue(GetLineNumber());
            double g = green->Evaluate(sc)->GetRealValue(GetLineNumber());
            double b = blue->Evaluate(sc)->GetRealValue(GetLineNumber());
            double a = alpha ? alpha->Evaluate(sc)->GetRealValue(GetLineNumber()) : 1.0;
            return Item::CreateColor(Color4d(r, g, b, a));
        }

    }

    void InterpreterColor::SetRedExpression(std::shared_ptr<InterpreterExpression> inRed) {
        red = inRed;
        red->SetLocation(false);
    }

    void InterpreterColor::SetGreenExpression(std::shared_ptr<InterpreterExpression> inGreen) {
        green = inGreen;
        green->SetLocation(false);
    }

    void InterpreterColor::SetBlueExpression(std::shared_ptr<InterpreterExpression> inBlue) {
        blue = inBlue;
        blue->SetLocation(false);
    }

    void InterpreterColor::SetAlphaExpression(std::shared_ptr<InterpreterExpression> inAlpha) {
        alpha = inAlpha;
        alpha->SetLocation(false);
    }

}
