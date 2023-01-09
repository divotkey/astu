/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/Interpreter.h"
#include "Velox/Interpreter/InterpreterError.h"
#include "Velox/Interpreter/InterpreterInstantDefinition.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/ItemStateFunction.h"
#include "Velox/Interpreter/InterpreterFunctionNoParameter.h"
#include "Velox/Interpreter/InterpreterFunctionOneParameter.h"
#include "Velox/Interpreter/InterpreterFunctionTwoParameter.h"
#include "Velox/Interpreter/InterpreterFunctionThreeParameter.h"
#include "Math/Random.h"
#include "Math/MathUtils.h"
#include "Graphics/ColorHsv.h"
#include "Graphics/Color.h"

// C++ Standard Library includes
#include <cassert>
#include <cmath>
#include <limits>

using namespace std;
using namespace astu;

namespace velox {

    Interpreter::Interpreter()
    {
        // Intentionally left empty.
    }

    void velox::Interpreter::Execute(InterpreterStatement &statement) {
        context.ClearFlags();
        context.ClearReturnValues();
        statement.Execute(context);
    }

    void Interpreter::AddGlobalFunction(const std::string &name, std::shared_ptr<InterpreterFunction> function) {
        if (context.HasGlobalItem(name)) {
            throw std::logic_error("Ambiguous function name '" + name + "'");
        }
        context.AddGlobalItem(name, Item::CreateFunction(function));
    }

    void Interpreter::AddGlobalReal(const string &name, double value) {
        context.AddGlobalItem(name, Item::CreateReal(value));
    }

    void Interpreter::AddGlobalInteger(const string &name, int value) {
        context.AddGlobalItem(name, Item::CreateInteger(value));
    }

    void Interpreter::AddGlobal(const string &name, std::shared_ptr<Item> item)
    {
        context.AddGlobalItem(name, item);
    }

    void Interpreter::AddInstant(InterpreterInstantDefinition &instantDef)
    {
        AddObjectType(instantDef.GetTypeName(), instantDef.CreateObjectType());
    }

    void Interpreter::AddObjectType(const std::string &name, std::shared_ptr<ObjectType> objType) {
        context.AddObjectType(name, objType);
    }

    [[maybe_unused]] bool Interpreter::HasObjectType(const string &name) const {
        return context.HasObjectType(name);
    }

    void Interpreter::AddStandardGlobals() {
        assert(context.NumGlobalScopes() > 0);

        AddGlobalReal("PI", MathUtils::PId);
        AddGlobalReal("PI2", MathUtils::PI2d);
        AddGlobalReal("MAX_REAL", std::numeric_limits<double>::max());
        AddGlobalReal("MIN_REAL", std::numeric_limits<double>::lowest());
        AddGlobalInteger("MAX_INT", std::numeric_limits<int>::max());
        AddGlobalInteger("MIN_INT", std::numeric_limits<int>::lowest());

        AddGlobalFunction("real", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    if (param->GetType() == ItemType::Real)
                        return param;
                    else
                        return Item::CreateReal(param->GetRealValue(lineNumber));
                }));

        AddGlobalFunction("int", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    if (param->GetType() == ItemType::Integer)
                        return param;
                    else
                        return Item::CreateInteger(param->GetIntegerValue());
                }));

        AddGlobalFunction("rnd", make_shared<InterpreterFunctionNoParameter>(
                [](unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(Random::GetInstance().NextDouble());
                }));

        AddGlobalFunction("RandomInt", make_shared<InterpreterFunctionTwoParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param1, shared_ptr<Item> param2, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    if (param2->IsUndefined()) {
                        if (param1->IsUndefined()) {
                            return Item::CreateInteger(Random::GetInstance().NextInt());
                        } else {
                            int value = param1->GetIntegerValue(lineNumber);
                            if (value <= 0) {
                                return Item::CreateInteger(Random::GetInstance().NextInt(std::numeric_limits<int>::min(), 0));
                            } else {
                                return Item::CreateInteger(Random::GetInstance().NextInt(0, std::numeric_limits<int>::max()));
                            }
                        }
                    } else {
                        int low = param1->GetIntegerValue(lineNumber);
                        int high = param2->GetIntegerValue(lineNumber);
                        if (high < low) {
                            std::swap(low, high);
                        }
                        return Item::CreateInteger(Random::GetInstance().NextInt(low, high));
                    }
                }));

        AddGlobalFunction("sqrt", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::sqrt(param->GetRealValue(lineNumber)));
                }));

        AddGlobalFunction("log", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::log(param->GetRealValue(lineNumber)));
                }));

        AddGlobalFunction("sin", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::sin(param->GetRealValue(lineNumber)));
                }));

        AddGlobalFunction("cos", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::cos(param->GetRealValue(lineNumber)));
                }));

        AddGlobalFunction("tan", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::tan(param->GetRealValue(lineNumber)));
                }));

        AddGlobalFunction("asin", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::asin(param->GetRealValue(lineNumber)));
                }));

        AddGlobalFunction("acos", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::acos(param->GetRealValue(lineNumber)));
                }));

        AddGlobalFunction("atan", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::atan(param->GetRealValue(lineNumber)));
                }));

        AddGlobalFunction("atan2", make_shared<InterpreterFunctionTwoParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param1, shared_ptr<Item> param2,
                   unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(
                            std::atan2(param1->GetRealValue(lineNumber), param2->GetRealValue(lineNumber)));
                }));

        AddGlobalFunction("min", make_shared<InterpreterFunctionTwoParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param1, shared_ptr<Item> param2,
                   unsigned int lineNumber) -> std::shared_ptr<Item> {
                    if (param1->IsReal() || param2->IsReal()) {
                        return Item::CreateReal(
                                std::min(param1->GetRealValue(lineNumber), param2->GetRealValue(lineNumber)));
                    } else if (param1->IsInteger() && param2->IsInteger()) {
                        return Item::CreateInteger(
                                std::min(param1->GetIntegerValue(lineNumber), param2->GetIntegerValue(lineNumber)));
                    }
                    throw InterpreterError("min function not defined for these types", lineNumber);
                }));

        AddGlobalFunction("max", make_shared<InterpreterFunctionTwoParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param1, shared_ptr<Item> param2,
                   unsigned int lineNumber) -> std::shared_ptr<Item> {
                    if (param1->IsReal() || param2->IsReal()) {
                        return Item::CreateReal(
                                std::max(param1->GetRealValue(lineNumber), param2->GetRealValue(lineNumber)));
                    } else if (param1->IsInteger() && param2->IsInteger()) {
                        return Item::CreateInteger(
                                std::max(param1->GetIntegerValue(lineNumber), param2->GetIntegerValue(lineNumber)));
                    }
                    throw InterpreterError("max function not defined for these types", lineNumber);
                }));

        AddGlobalFunction("rad2deg", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(MathUtils::ToDegrees(param->GetRealValue(lineNumber)));
                }));

        AddGlobalFunction("deg2rad", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(MathUtils::ToRadians(param->GetRealValue(lineNumber)));
                }));

        AddGlobalFunction("extractHue", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    ColorHsv hsv(param->GetColorValue());
                    return Item::CreateReal(hsv.h);
                }));

        AddGlobalFunction("extractSaturation", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    ColorHsv hsv(param->GetColorValue());
                    return Item::CreateReal(hsv.s);
                }));

        AddGlobalFunction("extractBrightness", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    ColorHsv hsv(param->GetColorValue());
                    return Item::CreateReal(hsv.v);
                }));

        AddGlobalFunction("colorFromHue", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    ColorHsv hsv(param->GetRealValue(lineNumber), 1.0, 1.0);
                    return Item::CreateColor(hsv.ToRgb());
                }));

        AddGlobalFunction("colorFromHSV", make_shared<InterpreterFunctionThreeParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param1, shared_ptr<Item> param2, shared_ptr<Item> param3,
                   unsigned int lineNumber) -> std::shared_ptr<Item> {
                    ColorHsv hsv(param1->GetRealValue(lineNumber), param2->GetRealValue(lineNumber),
                                 param3->GetRealValue(lineNumber));
                    return Item::CreateColor(hsv.ToRgb());
                }));

        AddGlobalFunction("clamp", make_shared<InterpreterFunctionThreeParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param1, shared_ptr<Item> param2, shared_ptr<Item> param3,
                   unsigned int lineNumber) -> std::shared_ptr<Item> {
                    if (param1->IsReal() || param2->IsReal() || param3->IsReal()) {

                        return Item::CreateReal(MathUtils::Clamp(param1->GetRealValue(lineNumber),
                                                                 param2->GetRealValue(lineNumber),
                                                                 param3->GetRealValue(lineNumber)));
                    }

                    return Item::CreateInteger(MathUtils::Clamp(param1->GetIntegerValue(),
                                                                param2->GetIntegerValue(),
                                                                param3->GetIntegerValue()));
                }));

        AddGlobalFunction("error", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    throw InterpreterError(param->GetStringValue(sc), lineNumber);
                }));

    }

    void Interpreter::CallFunctionNoParam(Item &item)
    {
        InterpreterNoParameterList noParams;
        CallFunction(item, noParams);
    }

    void Interpreter::CallFunctionIntParam(Item &item, int value)
    {
        InterpreterItemParameterList params;
        params.AddParameter(Item::CreateInteger(value));
        CallFunction(item, params);
    }

    void Interpreter::CallFunctionRealParam(Item &item, double value)
    {
        InterpreterItemParameterList params;
        params.AddParameter(Item::CreateReal(value));
        CallFunction(item, params);
    }

    void Interpreter::CallFunction(Item &item, InterpreterActualParameterList &params)
    {
        context.ClearFlags();
        context.ClearReturnValues();

        context.PushCodeBlockScope();
        item.CallAsFunction(context, params, 0);
        context.PopLocalScope();
    }

    void Interpreter::PushGlobalScope()
    {
        context.PushGlobalScope();
    }

    void Interpreter::PopGlobalScope()
    {
        context.PopGlobalScope();
    }

    size_t Interpreter::NumGlobalScopes() const
    {
        return context.NumGlobalScopes();
    }

    size_t Interpreter::NumLocalScopes() const
    {
        return context.NumLocalScopes();
    }

    void Interpreter::ClearLocalScopes()
    {
        context.ClearLocalScopes();
    }

} // end of namespace
