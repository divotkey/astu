#include "Item.h"
#include "ItemState.h"
#include "ItemStateReference.h"
#include "InterpreterError.h"
#include "IMemoryManager.h"


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
    ItemState::CallAsFunction(ScriptContext &sc, InterpreterActualParameterList &parameters, unsigned int lineNumber) {
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

    std::string ItemState::GetStringValue(ScriptContext &sc) const {
        throw InterpreterError("Not a string");
    }

    const Color4d & ItemState::GetColorValue() const {
        throw InterpreterError("Not a color");
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

    void ItemState::SwitchState(Item &item, std::unique_ptr<ItemState> newState) {
        item.state = move(newState);
    }

    std::shared_ptr<Item> ItemState::FindItem(const string &name) const {
        return nullptr;
    }

    bool ItemState::AddItem(const string &name, std::shared_ptr<Item> item) {
        throw InterpreterError("This type of value does not allow to add members.");
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

}