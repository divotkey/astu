#pragma once

#include "InterpreterFunction.h"

namespace velox {

    class InterpreterSineFunction : public InterpreterFunction {
    public:

        InterpreterSineFunction();

    protected:
        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc) override;
    };

}



