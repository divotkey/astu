#pragma once

#include "InterpreterExpression.h"
#include "InterpreterConstructorCall.h"

#include <string>

namespace velox {

    class InterpreterNewStatement : public InterpreterExpression {
    public:

        InterpreterNewStatement(unsigned int lineNumber) : InterpreterExpression(lineNumber) {}

        void SetTypeName(const std::string& name);
        void SetConstructorCall(std::shared_ptr<InterpreterConstructorCall> call);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        std::string typeName;
        std::shared_ptr<InterpreterConstructorCall> constructorCall;
    };

}



