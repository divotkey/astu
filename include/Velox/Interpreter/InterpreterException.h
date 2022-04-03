#pragma once

#include <stdexcept>

class InterpreterException : public std::runtime_error {
public:
    InterpreterException(const std::string& message) : std::runtime_error(message) {}
};
