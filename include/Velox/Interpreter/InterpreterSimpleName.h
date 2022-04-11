#pragma once

#include "InterpreterExpression.h"

#include <string>

namespace velox {

    class InterpreterSimpleName : public InterpreterExpression {
    public:

        InterpreterSimpleName(const std::string & name, unsigned int lineNumber)
            : InterpreterExpression(lineNumber, true), name(name) {}

        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;
        const std::string& GetName() const {
            return name;
        }

    private:
        std::string name;
    };

}


