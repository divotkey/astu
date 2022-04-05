#include "ItemStateBool.h"

using namespace std;

namespace velox {

    std::unique_ptr<ItemState> ItemStateBool::Copy() const {
        return make_unique<ItemStateBool>(value);
    }

    double ItemStateBool::GetRealValue() const {
        return static_cast<double>(value);
    }

    int ItemStateBool::GetIntegerValue() const {
        return static_cast<int>(value);
    }

    bool ItemStateBool::GetBooleanValue() const {
        return value;
    }

    string ItemStateBool::GetStringValue() const {
        return (value ? "true" : "false");
    }

    ItemType ItemStateBool::GetType() const {
        return ItemType::Boolean;
    }

}