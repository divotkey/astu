#include "ItemStateUndefined.h"

using namespace std;

namespace velox {

    unique_ptr<ItemState> ItemStateUndefined::Copy() const {
        return make_unique<ItemStateUndefined>();
    }

    string ItemStateUndefined::GetStringValue() const {
        return "undefined";
    }

    ItemType ItemStateUndefined::GetType() const {
        return ItemType::Undefined;
    }

}
