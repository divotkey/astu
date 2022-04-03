#pragma once

//#include "SimpleNameInterpreterNode.h"
#include <vector>


namespace velox {

    class InterpreterFormalParameterList {
    public:

        size_t NumParameters() const;
        const std::string &GetParameter(size_t idx) const;

    private:
        std::vector<std::string> parameters;
    };

}



