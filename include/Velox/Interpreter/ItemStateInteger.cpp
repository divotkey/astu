#include "ItemStateInteger.h"

using namespace std;

namespace velox {

    unique_ptr<ItemState> ItemStateInteger::Copy() const {
        return make_unique<ItemStateInteger>(value);
    }

    double ItemStateInteger::GetRealValue() const {
        return static_cast<double>(value);
    }

    int ItemStateInteger::GetIntegerValue() const {
        return value;
    }

    std::string ItemStateInteger::GetStringValue(ScriptContext &sc) const {
        return to_string(value);
    }

    ItemType ItemStateInteger::GetType() const {
        return ItemType::Integer;
    }

    bool ItemStateInteger::Assign(Item &owner, const ItemState &rhs) {
        if (rhs.GetType() != ItemType::Integer)
            return false;

        value = rhs.GetIntegerValue();
        return true;
    }

    //void *ItemStateInteger::operator new(size_t count) {
    //    return gMemoryManager->Allocate(count);
    //}
    //
    //void ItemStateInteger::operator delete(void *p) {
    //    gMemoryManager->Free(p);
    //}

}