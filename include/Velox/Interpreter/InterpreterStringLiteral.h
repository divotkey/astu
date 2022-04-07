#pragma once

#include "InterpreterExpression.h"

namespace velox {

    class InterpreterStringLiteral : public InterpreterExpression {
    public:

        InterpreterStringLiteral(const std::string &value);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        std::string value;
    };

}


