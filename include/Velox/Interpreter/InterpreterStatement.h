#pragma once

#include "ScriptContext.h"

namespace velox {

    class InterpreterStatement {
    public:

        ~InterpreterStatement() {}

        virtual void Execute(ScriptContext &sc) = 0;

    };

}


