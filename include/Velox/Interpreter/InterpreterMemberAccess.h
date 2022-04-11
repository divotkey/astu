#pragma once

#include "InterpreterExpression.h"
#include <string>

namespace velox {

    class InterpreterMemberAccess : public InterpreterExpression {
    public:

        InterpreterMemberAccess(unsigned int lineNumber) : InterpreterExpression(lineNumber, true) { }

        void SetLeftHandSide(std::shared_ptr<InterpreterExpression> lhs);
        void SetRightHandSide(const std::string& name);

        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        std::shared_ptr<InterpreterExpression> leftHandSide;
        std::string name;
    };

}


