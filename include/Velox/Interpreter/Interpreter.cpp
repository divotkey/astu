// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Interpreter.h"
#include "Item.h"
#include "ItemStateFunction.h"
#include "InterpreterFunctionNoParameter.h"
#include "InterpreterFunctionOneParameter.h"
#include "InterpreterFunctionTwoParameter.h"
#include "Math/Random.h"

// C++ Standard Library includes
#include <cmath>

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
        auto item = Item::Create(make_unique<ItemStateFunction>(function));
        superGlobals->AddItem(name, item);
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

        AddFunction("real", make_shared<InterpreterFunctionOneParameter>(
                [](shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    if (param->GetType() == ItemType::Real)
                        return param;
                    else
                        return Item::CreateReal(param->GetRealValue());
                }));

        AddFunction("int", make_shared<InterpreterFunctionOneParameter>(
                [](shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
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
                [](shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::sqrt(param->GetRealValue()));
                }));

        AddFunction("log", make_shared<InterpreterFunctionOneParameter>(
                [](shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::log(param->GetRealValue()));
                }));

        AddFunction("sin", make_shared<InterpreterFunctionOneParameter>(
                [](shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::sin(param->GetRealValue()));
                }));

        AddFunction("cos", make_shared<InterpreterFunctionOneParameter>(
                [](shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::cos(param->GetRealValue()));
                }));

        AddFunction("tan", make_shared<InterpreterFunctionOneParameter>(
                [](shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(std::tan(param->GetRealValue()));
                }));

    }

}
