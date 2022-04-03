#pragma once

#include "InterpreterExpression.h"
#include <vector>

namespace velox {

    class InterpreterActualParameterList {
    public:

        size_t NumParameters() const {
            return parameters.size();
        }

        void AddParameter(std::shared_ptr<InterpreterExpression> param);

        InterpreterExpression& GetParameter(size_t idx);
        const InterpreterExpression& GetParameter(size_t idx) const;

    private:
        std::vector<std::shared_ptr<InterpreterExpression>> parameters;
    };

}


