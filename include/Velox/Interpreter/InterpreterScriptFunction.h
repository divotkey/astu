#pragma once

#include "InterpreterFormalParameterList.h"
#include "InterpreterActualParameterList.h"
#include "InterpreterStatementBlock.h"
#include "Item.h"
#include "ScriptContext.h"
#include <memory>

namespace velox {

    class InterpreterScriptFunction {
    public:

        std::shared_ptr<Item> Execute(ScriptContext &sc, InterpreterActualParameterList &actualParameters);

    private:
        std::shared_ptr<InterpreterFormalParameterList> formalParameters;
        std::shared_ptr<InterpreterStatementBlock> statements;
    };

}


