#include "ObjectType.h"
#include "Item.h"
#include "ItemStateObject.h"
#include "InterpreterError.h"

#include <cassert>

using namespace std;

namespace velox {

    std::shared_ptr<Item> ObjectType::CreateObject(ScriptContext &sc) {
        auto result = Item::Create(make_unique<ItemStateObject>());

        for (auto it : items) {
            result->AddItem(it.first, it.second->Copy());
        }

        return result;
    }

    bool ObjectType::HasItem(const string &name) const {
        return items.find(name) != items.end();
    }

    void ObjectType::AddItem(const string &name, std::shared_ptr<Item> item) {
        assert(!HasItem(name));
        items[name] = item;
    }

}