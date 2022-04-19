// Local includes
#include "InterpreterStatementGlobal.h"
#include "InterpreterError.h"
#include "Item.h"

namespace velox {

    void InterpreterStatementGlobal::Execute(ScriptContext &sc) {
        if (sc.FindLocalItem(name)) {
            throw InterpreterError("variable '" + name + "' is already declared within local scope", GetLineNumber());
        }

        auto item = sc.FindGlobalItem(name);
        if (!item) {
            item = Item::CreateUndefined();
            sc.AddGlobalItem(name, item);
        }

        if (sc.FindLocalItem(name)) {
            // It seems that there is no local scope and hence we are done.
             return;
        }

        sc.AddItem(name, item);
    }

    void InterpreterStatementGlobal::SetName(const std::string &inName) {
        name = inName;
    }
}