#include "InterpreterActualParameterList.h"

#include <cassert>

namespace velox  {

    void InterpreterActualParameterList::AddParameter(std::shared_ptr<InterpreterExpression> param) {
        parameters.push_back(param);
    }

    InterpreterExpression &InterpreterActualParameterList::GetParameter(size_t idx) {
        assert(idx < parameters.size());
        return *parameters[idx];
    }

    const InterpreterExpression &InterpreterActualParameterList::GetParameter(size_t idx) const {
        assert(idx < parameters.size());
        return *parameters[idx];
    }

}
