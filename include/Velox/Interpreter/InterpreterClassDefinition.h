#pragma once

#include "InterpreterStatement.h"
#include "InterpreterFunctionDefinition.h"

#include <string>
#include <vector>
#include <memory>

namespace velox {

    class InterpreterClassDefinition : public InterpreterStatement {
    public:

        InterpreterClassDefinition(const std::string & name) : typeName(name) {}
        void AddFunction(std::shared_ptr<InterpreterFunctionDefinition>);
        bool HasFunction(const std::string& name) const;

        void Execute(ScriptContext &sc) override;

        void Prepare(ScriptContext &sc) override;

    private:
        std::string typeName;
        std::vector<std::shared_ptr<InterpreterFunctionDefinition>> functions;
    };

}


