#pragma once

#include "InterpreterFunction.h"

namespace velox {

    class InterpreterSineFunction : public InterpreterFunction {
    public:

        InterpreterSineFunction();

    protected:
        std::shared_ptr<Item> Execute(ScriptContext &sc) override;
    };

}



