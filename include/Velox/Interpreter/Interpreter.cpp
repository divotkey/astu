// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Interpreter.h"
#include "InterpreterError.h"
#include "Item.h"
#include "ItemStateFunction.h"
#include "InterpreterFunctionNoParameter.h"
#include "InterpreterFunctionOneParameter.h"
#include "InterpreterFunctionTwoParameter.h"
#include "InterpreterFunctionThreeParameter.h"
#include "Math/Random.h"
#include "Math/MathUtils.h"

// C++ Standard Library includes
#include <cmath>
#include <limits>

using namespace std;
using namespace astu;

namespace velox {

    Interpreter::Interpreter() : superGlobals(make_shared<Scope>()) {
        AddStandardFunctions();
    }

    void velox::Interpreter::Execute(std::shared_ptr<velox::InterpreterStatement> program) {
        context.ClearFlags();
        program->Execute(context);
    }

    void Interpreter::AddFunction(const std::string &name, std::shared_ptr<InterpreterFunction> function) {
        if (superGlobals->HasItem(name)) {
            throw std::logic_error("Ambiguous function name '" + name + "'");
        }
        superGlobals->AddItem(name, Item::CreateFunction(function));
    }

    void Interpreter::AddConstant(const string &name, double value) {
        superGlobals->AddItem(name, Item::CreateReal(value));
    }

    void Interpreter::AddObjectType(const std::string &name, std::shared_ptr<ObjectType> objType) {
        context.AddObjectType(name, objType);
    }

    bool Interpreter::HasObjectType(const string &name) const {
        return context.HasObjectType(name);
    }

    void Interpreter::ClearVariables() {
        context.Clear();
        context.PushScope(superGlobals);
    }

    void Interpreter::ClearAll() {
        superGlobals->Clear();
        ClearVariables();
    }

    void Interpreter::AddStandardFunctions() {

        AddConstant("PI", MathUtils::PId);
        AddConstant("PI2", MathUtils::PI2d);
        AddConstant("MAX_REAL", std::numeric_limits<double>::max());
        AddConstant("MIN_REAL", std::numeric_limits<double>::lowest());

        AddFunction("real", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    if (param->GetType() == ItemType::Real)
                        return param;
                    else
                        return Item::CreateReal(param->GetRealValue(lineNumber));
                }));

        AddFunction("int", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    if (param->GetType() == ItemType::Integer)
                        return param;
                    else
                        return Item::CreateInteger(param->GetIntegerValue());
                }));

        AddFunction("rnd", make_shared<InterpreterFunctionNoParameter>(
                [](unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(Random::GetInstance().NextDouble());
                }));

        AddFunction("sqrt", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::sqrt(param->GetRealValue(lineNumber)));
                }));

        AddFunction("log", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::log(param->GetRealValue(lineNumber)));
                }));

        AddFunction("sin", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::sin(param->GetRealValue(lineNumber)));
                }));

        AddFunction("cos", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::cos(param->GetRealValue(lineNumber)));
                }));

        AddFunction("tan", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::tan(param->GetRealValue(lineNumber)));
                }));

        AddFunction("asin", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::asin(param->GetRealValue(lineNumber)));
                }));

        AddFunction("acos", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::acos(param->GetRealValue(lineNumber)));
                }));

        AddFunction("atan", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::atan(param->GetRealValue(lineNumber)));
                }));

        AddFunction("atan2", make_shared<InterpreterFunctionTwoParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param1, shared_ptr<Item> param2, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(
                            std::atan2(param1->GetRealValue(lineNumber), param2->GetRealValue(lineNumber)));
                }));

        AddFunction("min", make_shared<InterpreterFunctionTwoParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param1, shared_ptr<Item> param2, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    if (param1->IsReal() || param2->IsReal()){
                        return Item::CreateReal(
                                std::min(param1->GetRealValue(lineNumber), param2->GetRealValue(lineNumber)));
                    } else if (param1->IsInteger() && param2->IsInteger()) {
                        return Item::CreateInteger(
                                std::min(param1->GetIntegerValue(lineNumber), param2->GetIntegerValue(lineNumber)));
                    }
                    throw InterpreterError("min function not defined for these types", lineNumber);
                }));

        AddFunction("max", make_shared<InterpreterFunctionTwoParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param1, shared_ptr<Item> param2, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    if (param1->IsReal() || param2->IsReal()){
                        return Item::CreateReal(
                                std::max(param1->GetRealValue(lineNumber), param2->GetRealValue(lineNumber)));
                    } else if (param1->IsInteger() && param2->IsInteger()) {
                        return Item::CreateInteger(
                                std::max(param1->GetIntegerValue(lineNumber), param2->GetIntegerValue(lineNumber)));
                    }
                    throw InterpreterError("max function not defined for these types", lineNumber);
                }));

        AddFunction("rad2deg", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(MathUtils::ToDegrees(param->GetRealValue(lineNumber)));
                }));

        AddFunction("deg2rad", make_shared<InterpreterFunctionOneParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(MathUtils::ToRadians(param->GetRealValue(lineNumber)));
                }));

        AddFunction("clamp", make_shared<InterpreterFunctionThreeParameter>(
                [](ScriptContext &sc, shared_ptr<Item> param1, shared_ptr<Item> param2, shared_ptr<Item> param3, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    if (param1->IsReal() || param2->IsReal() || param3->IsReal()) {

                        return Item::CreateReal(MathUtils::Clamp(param1->GetRealValue(lineNumber),
                                                                 param2->GetRealValue(lineNumber),
                                                                 param3->GetRealValue(lineNumber)));
                    }

                    return Item::CreateInteger(MathUtils::Clamp(param1->GetIntegerValue(),
                                                             param2->GetIntegerValue(),
                                                             param3->GetIntegerValue()));

                    return Item::CreateUndefined();
                }));
    }

}
