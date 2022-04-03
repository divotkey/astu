#include "InterpreterFormalParameterList.h"

#include <cassert>

namespace velox {

    size_t InterpreterFormalParameterList::NumParameters() const {
        return parameters.size();
    }

    const std::string &InterpreterFormalParameterList::GetParameter(size_t idx) const {
        assert(idx < parameters.size());
        return parameters[idx];
    }

}
