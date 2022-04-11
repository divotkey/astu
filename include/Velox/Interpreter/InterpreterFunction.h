#pragma once

#include "InterpreterFormalParameterList.h"
#include "Item.h"

namespace velox {

    class InterpreterFunction {
    public:

        virtual ~InterpreterFunction() {}

        std::shared_ptr<Item>
        Evaluate(ScriptContext &sc, InterpreterActualParameterList &actualParameters, unsigned int lineNumber);
        void AddFormalParameter(const std::string& simpleName);
        bool HasFormalParameter(const std::string& simpleName) const;

    protected:
        virtual std::shared_ptr<Item> DoEvaluate(ScriptContext &sc) = 0;

    private:
        std::vector<std::string> formalParameters;
    };

}


