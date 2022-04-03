#include "Interpreter.h"

namespace velox {

    Interpreter::Interpreter() {

    }

    void velox::Interpreter::Execute(std::shared_ptr<velox::InterpreterStatement> program) {
        program->Execute(context);
    }

}
