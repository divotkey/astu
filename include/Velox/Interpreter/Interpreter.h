#pragma once

// Local includes
#include "InterpreterStatement.h"
#include "ScriptContext.h"

// C++ Standard Library includes
#include <stdexcept>
#include <memory>


namespace velox {

    class Interpreter {

    public:

        Interpreter();

        void Execute(std::shared_ptr<InterpreterStatement> program);

    private:
        ScriptContext context;
    };

}
