#pragma once

#include "InterpreterStatement.h"
#include "InterpreterScriptFunction.h"
#include "InterpreterSimpleName.h"

namespace velox {

    class InterpreterFunctionDefinition : public InterpreterStatement {
    public:

        void SetFunction(std::shared_ptr<InterpreterScriptFunction> function);
        void SetFunctionName(const std::string& name);
        const std::string& GetFunctionName() const;

        std::shared_ptr<Item> CreateFunctionItem() const;

        // Inherited via InterpreterStatement
        void Execute(ScriptContext &sc) override;

        void Prepare(ScriptContext &sc) override;

    private:
        std::shared_ptr<InterpreterScriptFunction> function;
        std::string functionName;
    };

}



