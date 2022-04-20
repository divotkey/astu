// Local includes
#include "ItemStateColor.h"
#include "InterpreterFunction.h"
#include "InterpreterError.h"
#include "Item.h"
#include "InterpreterFunctionTwoParameter.h"

// C++ Standard Library includes.
#include <functional>

using namespace std;
using namespace astu;

namespace velox {

    /**
     * Color function with no parameter.
     */
    class ColorFunction0Param : public InterpreterFunction {
    public:

        ColorFunction0Param(function<std::shared_ptr<Item> (void)> func) : func(func) { }

    protected:

        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override {
            return func();
        }

    private:
        function<std::shared_ptr<Item> (void)> func;
    };

    /**
     * Color function with one parameter.
     */
    class ColorFunction1Param : public InterpreterFunction {
    public:

        ColorFunction1Param(function<std::shared_ptr<Item> (const Item &rhs, unsigned intLineNumber)> func) : func(func) {
            AddFormalParameter("a");
        }

    protected:

        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override {
            return func(sc.GetItem("a"), lineNumber);
        }

    private:
        function<std::shared_ptr<Item> (const Item &rhs, unsigned intLineNumber)> func;
    };

    class SaturateFunction : public InterpreterFunction {
    public:

        SaturateFunction(Color4d &color) : color(color) {}

    protected:

        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override {
            color.Saturate();
            return nullptr;
        }

    private:
        Color4d &color;
    };

    class ItemStateColorMember : public ItemState {
    public:
        ItemStateColorMember(double &value) : value(value) {}

        // Inherited via ItemState
    private:
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
        auto &rColor = value;

        AddItem("Saturate", Item::CreateFunction(make_shared<SaturateFunction>(value)));

        AddItem("Lerp", InterpreterFunctionTwoParameter::CreateItem(
                [&rColor](ScriptContext &sc, std::shared_ptr<Item> param1, std::shared_ptr<Item> param2,
                          unsigned int lineNumber) -> std::shared_ptr<Item> {
                    if (param1->GetType() != ItemType::Color) {
                        throw InterpreterError("first parameter for method 'Lerp' must be of type color", lineNumber);
                    }

                    if (param1->IsNumber()) {
                        throw InterpreterError("second parameter for method 'Lerp' must be a number", lineNumber);
                    }

                    return Item::CreateColor(rColor.Lerp(param1->GetColorValue(), param2->GetRealValue(lineNumber)));
                }));

        AddItem("operator+", Item::CreateFunction(
                make_shared<ColorFunction1Param>([&rColor](const Item &item, unsigned int lineNumber) {
            if (item.GetType() == ItemType::Color) {
                return Item::CreateColor(rColor + item.GetColorValue());
            }
            throw InterpreterError("invalid operation for type color", lineNumber);
        })));

        AddItem("operator-", Item::CreateFunction(
                make_shared<ColorFunction1Param>([&rColor](const Item &item, unsigned int lineNumber) {
                    if (item.GetType() == ItemType::Color) {
                        return Item::CreateColor(rColor - item.GetColorValue());
                    }
                    throw InterpreterError("invalid operation for type color", lineNumber);
                })));

        AddItem("operator*", Item::CreateFunction(
                make_shared<ColorFunction1Param>([&rColor](const Item &item, unsigned int lineNumber) {
                    if (item.GetType() == ItemType::Color) {
                        return Item::CreateColor(rColor * item.GetColorValue());
                    } else if (item.GetType() == ItemType::Real || item.GetType() == ItemType::Integer) {
                        return Item::CreateColor(rColor * item.GetRealValue(lineNumber));
                    }
                    throw InterpreterError("invalid operation for type color", lineNumber);
                })));

        AddItem("operator/", Item::CreateFunction(
                make_shared<ColorFunction1Param>([&rColor](const Item &item, unsigned int lineNumber) {
                    if (item.GetType() == ItemType::Real || item.GetType() == ItemType::Integer) {
                        return Item::CreateColor(rColor / item.GetRealValue(lineNumber));
                    }
                    throw InterpreterError("invalid operation for type color", lineNumber);
                })));

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
        return ItemType::Color;
    }

    bool ItemStateColor::Assign(Item &owner, const ItemState &rhs) {
        if (rhs.GetType() != ItemType::Color)
            return false;

        value = rhs.GetColorValue();
        return true;
    }

}