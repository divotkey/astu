#pragma once

#include "InterpreterFormalParameterList.h"
#include "Item.h"

namespace velox {

    class InterpreterFunction {
    public:

        std::shared_ptr<Item> Execute(ScriptContext &sc, InterpreterActualParameterList &actualParameters);

    protected:

        void AddFormalParameter(const std::string& simpleName);
        bool HasFormalParameter(const std::string& simpleName) const;

        virtual std::shared_ptr<Item> Execute(ScriptContext &sc) = 0;

    private:
        std::vector<std::string> formalParameters;
    };

}


