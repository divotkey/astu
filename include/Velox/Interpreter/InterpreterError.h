#pragma once

#include <stdexcept>

class InterpreterError : public std::runtime_error {
public:
    InterpreterError(const std::string& message) : std::runtime_error(message) {}
};
