#pragma once

#include "InterpreterFunction.h"

namespace velox {

    class InterpreterFunctionPrint : public InterpreterFunction {
    public:

        InterpreterFunctionPrint();

    protected:
        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override;
    };

}



