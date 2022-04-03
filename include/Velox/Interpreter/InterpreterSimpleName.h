#pragma once

#include "InterpreterExpression.h"

#include <string>

namespace velox {

    class InterpreterSimpleName : public InterpreterExpression {
    public:

        InterpreterSimpleName(const std::string & name, bool location)
            : InterpreterExpression(location), name(name) {}

        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        std::string name;
    };

}


