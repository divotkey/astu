#include "ItemState.h"
#include "InterpreterException.h"

namespace velox {

    std::shared_ptr<Item> ItemState::CallAsFunction(ScriptContext &sc, InterpreterActualParameterList &parameters) {
        throw InterpreterException("Item cannot be called as function");
    }

    double ItemState::GetRealValue() const {
        throw InterpreterException("Item cannot be interpreted as real value");
    }

    int ItemState::GetIntegerValue() const {
        throw InterpreterException("Item cannot be interpreted as integer value");
    }

    std::string ItemState::GetStringValue() const {
        throw InterpreterException("Item cannot be interpreted as string value");
    }

    ItemType ItemState::GetType() const {
        return ItemType::Other;
    }

}