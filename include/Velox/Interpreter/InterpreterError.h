#pragma once

#include <stdexcept>

namespace velox {

    class InterpreterError : public std::runtime_error {
    public:

        InterpreterError(const std::string& message, unsigned int line = 0)
                : std::runtime_error(message), lineNumber(line) {
            // Intentionally left empty.
        }

        virtual ~InterpreterError() {}

        unsigned int GetLineNumber() const {
            return lineNumber;
        }

    private:
        unsigned int lineNumber;

    };

}

