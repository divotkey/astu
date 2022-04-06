#include "ItemStateInteger.h"

using namespace std;

namespace velox {

    std::unique_ptr<ItemState> ItemStateInteger::Copy() const {
        return make_unique<ItemStateInteger>(value);
    }

    double ItemStateInteger::GetRealValue() const {
        return static_cast<double>(value);
    }

    int ItemStateInteger::GetIntegerValue() const {
        return value;
    }

    string ItemStateInteger::GetStringValue() const {
        return to_string(value);
    }

    ItemType ItemStateInteger::GetType() const {
        return ItemType::Integer;
    }

    //void *ItemStateInteger::operator new(size_t count) {
    //    return gMemoryManager->Allocate(count);
    //}
    //
    //void ItemStateInteger::operator delete(void *p) {
    //    gMemoryManager->Free(p);
    //}

}