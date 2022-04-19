// Local includes
#include "InterpreterStatementBreak.h"

namespace velox {

    void InterpreterStatementBreak::Execute(ScriptContext &sc) {
        sc.SetFlag(ScriptContext::BREAK_EXECUTED_FLAG);
    }

}