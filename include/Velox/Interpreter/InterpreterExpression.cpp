#include "InterpreterExpression.h"

namespace velox {

    void InterpreterExpression::Execute(ScriptContext &sc) {
        // Evaluate this expression and ignore the result.
        Evaluate(sc);
    }

}
