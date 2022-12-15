#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/ItemState.h"
#include "Velox/Interpreter/ItemStateReference.h"
#include "Velox/Interpreter/InterpreterError.h"
#include "Velox/Interpreter/IMemoryManager.h"


// C++ Standard Library includes
#include <stdexcept>

using namespace std;
using namespace astu;

namespace velox {

    void ItemState::Assign(Item &owner, std::shared_ptr<Item> rhs) {
        if (Assign(owner, *rhs->state))
            return;

        if (rhs->state->GetType() == ItemType::Other) {
            owner.state = make_unique<ItemStateReference>(rhs);
        } else {
            owner.state = rhs->state->Copy();
        }
    }

    std::shared_ptr<Item>
    ItemState::CallAsFunction(ScriptContext &sc, InterpreterActualParameterList &parameters, std::shared_ptr<Scope> memberScope, unsigned int lineNumber) {
        throw InterpreterError("Not a function");
    }

    double ItemState::GetRealValue(unsigned int lineNumber) const {
        throw InterpreterError("floating-point value expected", lineNumber);
    }

    int ItemState::GetIntegerValue(unsigned int lineNumber) const {
        throw InterpreterError("integer value expected", lineNumber);
    }

    bool ItemState::GetBooleanValue(unsigned int lineNumber) const {
        throw InterpreterError("boolean value expected", lineNumber);
    }

    std::string ItemState::GetStringValue(ScriptContext &sc) const {
        throw InterpreterError("string value expected");
    }

    const Color4d & ItemState::GetColorValue() const {
        throw InterpreterError("color value expected");
    }

    const astu::Vector2d &ItemState::GetVector2Value() const {
        throw InterpreterError("vector2 value expected");
    }

    void *ItemState::operator new(size_t count) {
        return gMemoryManager->Allocate(count);
    }

    void ItemState::operator delete(void *p) {
        gMemoryManager->Free(p);
    }

    void ItemState::SwitchState(Item &item, std::unique_ptr<ItemState> newState) {
        item.state = move(newState);
    }

    std::shared_ptr<Item> ItemState::FindItem(const string &name) const {
        return nullptr;
    }

    std::shared_ptr<Item> ItemState::GetListElement(size_t idx, unsigned int lineNumber) {
        throw InterpreterError("Not a list", lineNumber);
    }

    void ItemState::AppendListElement(std::shared_ptr<Item> elem) {
        throw InterpreterError("Not a list");
    }

    bool ItemState::AddItem(const string &name, std::shared_ptr<Item> item) {
        throw InterpreterError("This type does not allow to add members.");
    }

    std::shared_ptr<Item> ItemState::GetParent(Item &context) {
        return context.parent.lock();
    }

    std::shared_ptr<Item> ItemState::GetReferencedItem() {
        return nullptr;
    }

    void ItemState::SetData(shared_ptr<ItemData>) {
        throw std::logic_error("Only items with object state can hold additional data");
    }

    shared_ptr<ItemData> ItemState::GetData() {
        return nullptr;
    }

    std::shared_ptr<Item> ItemState::ExecuteUnaryMinus() const {
        throw InterpreterError(string("unary minus not defined for type ") + std::to_string(static_cast<int>(GetType())));
    }

    bool ItemState::IsReference() const {
        return false;
    }

}