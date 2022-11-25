#pragma once

// Local includes
#include "InterpreterExpression.h"

// C++ Standard Library includes
#include <vector>

namespace velox {

    class InterpreterActualParameterList {
    public:

        /** Destructor. */
        virtual ~InterpreterActualParameterList() {}

        virtual size_t NumParameters() = 0;
        virtual std::shared_ptr<Item> EvaluateParam(ScriptContext &sc, size_t idx) = 0;
    };

    class InterpreterExpressionParameterList : public InterpreterActualParameterList {
    public:

        void AddParameter(std::shared_ptr<InterpreterExpression> param);

        // Inherited via InterpreterActualParameterList
        size_t NumParameters() override;
        std::shared_ptr<Item> EvaluateParam(ScriptContext &sc, size_t idx) override;

    private:
        std::vector<std::shared_ptr<InterpreterExpression>> parameters;
    };

    class InterpreterItemParameterList : public InterpreterActualParameterList {
    public:

        void AddParameter(std::shared_ptr<Item> param);

        // Inherited via InterpreterActualParameterList
        size_t NumParameters() override;
        std::shared_ptr<Item> EvaluateParam(ScriptContext &sc, size_t idx) override;

    private:
        std::vector<std::shared_ptr<Item>> parameters;
    };

    class InterpreterNoParameterList : public InterpreterActualParameterList {
    public:

        // Inherited via InterpreterActualParameterList
        size_t NumParameters() override { return 0; }
        std::shared_ptr<Item> EvaluateParam(ScriptContext &sc, size_t idx) override;
    };


}


