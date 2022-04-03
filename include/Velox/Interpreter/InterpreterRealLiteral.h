#pragma once

#include "InterpreterExpression.h"

namespace velox {

    class InterpreterRealLiteral : public InterpreterExpression {
    public:
        InterpreterRealLiteral(double value);

        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        double value;
    };

}


