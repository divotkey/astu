// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "InterpreterColor.h"
#include "Item.h"

using namespace std;
using namespace astu;

namespace velox {

    shared_ptr<Item> InterpreterColor::Evaluate(ScriptContext &sc) {
        double r = red->Evaluate(sc)->GetRealValue();
        double g = green->Evaluate(sc)->GetRealValue();
        double b = blue->Evaluate(sc)->GetRealValue();
        double a = alpha ? alpha->Evaluate(sc)->GetRealValue() : 1.0;

        return Item::CreateColor(Color4d(r, g, b, a));
    }

    void InterpreterColor::SetRedExpression(std::shared_ptr<InterpreterExpression> inRed) {
        red = inRed;
    }

    void InterpreterColor::SetGreenExpression(std::shared_ptr<InterpreterExpression> inGreen) {
        green = inGreen;
    }

    void InterpreterColor::SetBlueExpression(std::shared_ptr<InterpreterExpression> inBlue) {
        blue = inBlue;
    }

    void InterpreterColor::SetAlphaExpression(std::shared_ptr<InterpreterExpression> inAlpha) {
        alpha = inAlpha;
    }

}
