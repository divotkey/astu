#pragma once

#include "InterpreterFunction.h"

namespace velox {

    class InterpreterPrintFunction : public InterpreterFunction {
    public:

        InterpreterPrintFunction();

    protected:
        std::shared_ptr<Item> Execute(ScriptContext &sc) override;
    };

}



