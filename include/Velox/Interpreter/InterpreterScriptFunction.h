#pragma once

#include "InterpreterStatementBlock.h"
#include "InterpreterFunction.h"
#include "Item.h"
#include "ScriptContext.h"
#include <memory>

namespace velox {

    class InterpreterScriptFunction : public InterpreterFunction {
    public:

        void SetStatement(std::shared_ptr<InterpreterStatement> statement);

    protected:
        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override;

    private:
        std::shared_ptr<InterpreterStatement> statement;
    };

}


