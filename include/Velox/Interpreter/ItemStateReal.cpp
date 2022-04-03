#include "ItemStateReal.h"

using namespace std;

namespace velox {

    std::unique_ptr<ItemState> ItemStateReal::Copy() const {
        return make_unique<ItemStateReal>(value);
    }

    double ItemStateReal::GetRealValue() const {
        return value;
    }

    int ItemStateReal::GetIntegerValue() const {
        return static_cast<int>(value);
    }

    string ItemStateReal::GetStringValue() const {
        return to_string(value);
    }

    ItemType ItemStateReal::GetType() const {
        return ItemType::Real;
    }
}