#pragma once

#include "InterpreterExpression.h"

namespace velox {

    class InterpreterIntegerLiteral : public InterpreterExpression {
    public:
        InterpreterIntegerLiteral(int value);

        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        int value;
    };

}


