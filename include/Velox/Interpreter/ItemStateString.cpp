#include "ItemStateString.h"
#include "IMemoryManager.h"

using namespace std;

namespace velox {

    // Alternative implementation using memory manager.
    //ItemStateString::ItemStateString(const string &inValue) {
    //    value = reinterpret_cast<char*>(gMemoryManager->Allocate(inValue.size() + 1));
    //    inValue.copy(value, inValue.size());
    //    value[inValue.size()] = 0;
    //}
    //
    //ItemStateString::~ItemStateString() {
    //    gMemoryManager->Free(value);
    //}

    ItemStateString::ItemStateString(const string &inValue) : value(inValue) {
        // Intentionally left empty.
    }

    ItemStateString::~ItemStateString() {
        // Intentionally left empty.
    }

    unique_ptr<ItemState> ItemStateString::Copy() const {
        return make_unique<ItemStateString>(value);
    }

    string ItemStateString::GetStringValue() const {
        return value;
    }

    ItemType ItemStateString::GetType() const {
        return ItemType::String;
    }

    //void *ItemStateString::operator new(size_t count) {
    //    return gMemoryManager->Allocate(count);
    //}
    //
    //void ItemStateString::operator delete(void *p) {
    //    gMemoryManager->Free(p);
    //}

}