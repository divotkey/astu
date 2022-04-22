// Local includes
#include "ItemStateColor.h"
#include "InterpreterFunction.h"
#include "InterpreterError.h"
#include "Item.h"
#include "InterpreterFunctionTwoParameter.h"
#include "InterpreterFunctionNoParameter.h"

// C++ Standard Library includes.
#include <functional>

using namespace std;
using namespace astu;

namespace velox {

    //class SaturateFunction : public InterpreterFunction {
    //public:
    //
    //    SaturateFunction(Color4d &color) : color(color) {}
    //
    //protected:
    //
    //    std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override {
    //        color.Saturate();
    //        return nullptr;
    //    }
    //
    //private:
    //    Color4d &color;
    //};

    //class LerpMethod : public InterpreterFunction {
    //public:
    //    LerpMethod(const Color4d &color) : color (color) {
    //        AddFormalParameter("c");
    //        AddFormalParameter("t");
    //    }
    //
    //protected:
    //    std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override {
    //        auto &c = sc.GetItem("c");
    //        if (c.GetType() != ItemType::Color4) {
    //            throw InterpreterError("first parameter for method 'Lerp' must be of type color", lineNumber);
    //        }
    //
    //        auto &t = sc.GetItem("t");
    //        if (!t.IsNumber()) {
    //            throw InterpreterError("second parameter for method 'Lerp' must be a number", lineNumber);
    //        }
    //
    //        return Item::CreateColor(color.Lerp(c.GetColorValue(), t.GetRealValue(lineNumber)));
    //    }
    //
    //private:
    //    const Color4d &color;
    //};

    class ItemStateColorMember : public ItemState {
    public:
        ItemStateColorMember(double &value) : value(value) {}

        // Inherited via ItemState
        unique_ptr<ItemState> Copy() const override {
            return make_unique<ItemStateReal>(value);
        }

        double GetRealValue(unsigned int lineNumber) const override {
            return value;
        }

        int GetIntegerValue(unsigned int lineNumber) const override {
            return static_cast<int>(value);
        }

        string GetStringValue(ScriptContext &sc) const override {
            return to_string(value);
        }

        bool Assign(Item &owner, const ItemState &rhs) override {
            value = rhs.GetRealValue(0);
            return true;
        }

        ItemType GetType() const override {
            return ItemType::Real;
        }

    private:
        double &value;
    };

    ItemStateColor::ItemStateColor(const Color4d &inValue) : value(inValue) {

        AddItem("Saturate", InterpreterFunctionNoParameter::CreateItem([this](unsigned int lineNumber) -> std::shared_ptr<Item> {
            this->value.Saturate();
            return Item::CreateUndefined();
        }));

        AddItem("Lerp", InterpreterFunctionTwoParameter::CreateItem(
                [this](ScriptContext &sc, std::shared_ptr<Item> param1, std::shared_ptr<Item> param2,
                          unsigned int lineNumber) -> std::shared_ptr<Item>
                {
                    if (param1->GetType() != ItemType::Color4) {
                        throw InterpreterError("first parameter for method 'Lerp' must be of type color", lineNumber);
                    }

                    if (!param2->IsNumber()) {
                        throw InterpreterError("second parameter for method 'Lerp' must be a number", lineNumber);
                    }

                    return Item::CreateColor(value.Lerp(param1->GetColorValue(), param2->GetRealValue(lineNumber)));
                }));

        AddItem("red", Item::Create(std::make_unique<ItemStateColorMember>(value.r)));
        AddItem("green", Item::Create(std::make_unique<ItemStateColorMember>(value.g)));
        AddItem("blue", Item::Create(std::make_unique<ItemStateColorMember>(value.b)));
        AddItem("alpha", Item::Create(std::make_unique<ItemStateColorMember>(value.a)));
    }

    unique_ptr<ItemState> ItemStateColor::Copy() const {
        return make_unique<ItemStateColor>(value);
    }

    int ItemStateColor::GetIntegerValue(unsigned int lineNumber) const {
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
        return ItemType::Color4;
    }

    bool ItemStateColor::Assign(Item &owner, const ItemState &rhs) {
        if (rhs.GetType() != ItemType::Color4)
            return false;

        value = rhs.GetColorValue();
        return true;
    }

}