// Local includes
#include "ItemStateColor.h"
#include "InterpreterFunction.h"

using namespace std;
using namespace astu;

namespace velox {

    class SaturateFunction : public InterpreterFunction {
    public:

    };

    ItemStateColor::ItemStateColor(const Color4d &value) : value(value) {

    }

    unique_ptr<ItemState> ItemStateColor::Copy() const {
        return make_unique<ItemStateColor>(value);
    }

    int ItemStateColor::GetIntegerValue() const {
        return value.GetARGB();
    }

    std::string ItemStateColor::GetStringValue(ScriptContext &sc) const {
        if (value.a < 1) {
            return "|" + to_string(value.r) + ", " + to_string(value.g) + ", " + to_string(value.b) + ", " +
                   to_string(value.a) + "|";
        }
        return "|" + to_string(value.r) + ", " + to_string(value.g) + ", " + to_string(value.b) + "|";
    }

    const Color4d & ItemStateColor::GetColorValue() const {
        return value;
    }

    ItemType ItemStateColor::GetType() const {
        return ItemType::Color;
    }

    bool ItemStateColor::Assign(Item &owner, const ItemState &rhs) {
        if (rhs.GetType() != ItemType::Color)
            return false;

        value = rhs.GetColorValue();
        return true;
    }

    shared_ptr<Item> ItemStateColor::FindItem(const string &name) {
        //if (name == "Saturate") {
        //
        //}

        return nullptr;
    }

}