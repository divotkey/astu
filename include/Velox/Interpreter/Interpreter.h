#pragma once

// Local includes
#include "InterpreterStatement.h"
#include "ScriptContext.h"
#include "Scope.h"

// C++ Standard Library includes
#include <stdexcept>
#include <memory>


namespace velox {

    class InterpreterFunction;

    class Interpreter {

    public:

        Interpreter();

        void AddFunction(const std::string& name, std::shared_ptr<InterpreterFunction> function);
        void Execute(std::shared_ptr<InterpreterStatement> program);
        void ClearVariables();
        void ClearAll();

    private:
        std::shared_ptr<Scope> superGlobals;
        ScriptContext context;
    };

}
