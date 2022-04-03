#include "ItemStateReference.h"
#include "Item.h"

using namespace std;

namespace velox {

    shared_ptr<Item>
    ItemStateReference::CallAsFunction(ScriptContext &sc, InterpreterActualParameterList &parameters) {
        return value->state->CallAsFunction(sc, parameters);
    }

    unique_ptr<ItemState> ItemStateReference::Copy() const {
        return value->state->Copy();
    }

    double ItemStateReference::GetRealValue() const {
        return value->state->GetRealValue();
    }

    int ItemStateReference::GetIntegerValue() const {
        return value->state->GetIntegerValue();
    }

    string ItemStateReference::GetStringValue() const {
        return value->state->GetStringValue();
    }

    ItemType ItemStateReference::GetType() const {
        return value->state->GetType();
    }

}
