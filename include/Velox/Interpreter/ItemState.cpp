#include "ItemState.h"
#include "InterpreterError.h"
#include "IMemoryManager.h"

namespace velox {

    std::shared_ptr<Item> ItemState::CallAsFunction(ScriptContext &sc, InterpreterActualParameterList &parameters) {
        throw InterpreterError("Not a function");
    }

    double ItemState::GetRealValue() const {
        throw InterpreterError("Not a floating-point value");
    }

    int ItemState::GetIntegerValue() const {
        throw InterpreterError("Not an integer");
    }

    bool ItemState::GetBooleanValue() const {
        throw InterpreterError("Not a boolean");
    }

    std::string ItemState::GetStringValue() const {
        throw InterpreterError("Not a string");
    }

    ItemType ItemState::GetType() const {
        return ItemType::Other;
    }

    void *ItemState::operator new(size_t count) {
        return gMemoryManager->Allocate(count);
    }

    void ItemState::operator delete(void *p) {
        gMemoryManager->Free(p);
    }

}