/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/ScriptContext.h"
#include "Velox/Interpreter/InterpreterError.h"
#include "Velox/Interpreter/Scope.h"

// C++ Standard Library includes.
#include <cassert>
#include <iostream>

//#define DEBUG_OUTPUT

using namespace std;

namespace velox {

    const unsigned int ScriptContext::RETURN_EXECUTED_FLAG      = 1 << 0;
    const unsigned int ScriptContext::BREAK_EXECUTED_FLAG       = 1 << 1;
    const unsigned int ScriptContext::CONTINUE_EXECUTED_FLAG    = 1 << 2;

    //void ScriptContext::PushScope(std::shared_ptr<Scope> scope) {
    //    if (!scope) {
    //        scope = std::make_shared<Scope>(false);
    //    }
    //    scopes.push_front(scope);
    //}
    //
    //void ScriptContext::PopScope() {
    //    scopes.pop_front();
    //}
    //
    //Scope &ScriptContext::GetCurrentScope() {
    //    assert(!scopes.empty());
    //    return *scopes.front();
    //}
    //
    //const Scope &ScriptContext::GetCurrentScope() const {
    //    assert(!scopes.empty());
    //    return *scopes.front();
    //}

    bool ScriptContext::HasItem(const std::string &name) const {
        return HasLocalItem(name) || HasGlobalItem(name);
    }

    Item &ScriptContext::GetItem(const std::string &name, unsigned int lineNumber) {
        auto item = FindItem(name);
        if (!item) {
            throw InterpreterError("Unknown identifier '" + name + "'", lineNumber);
        }

        return *item;
    }

    Item &ScriptContext::GetItem(const string &name)
    {
        auto item = FindItem(name);
        if (!item) {
            throw std::logic_error("Unknown identifier '" + name + "'");
        }

        return *item;
    }

    size_t ScriptContext::NumGlobalScopes() const
    {
        return globalScopes.size();
    }

    size_t ScriptContext::NumLocalScopes() const
    {
        return localScopes.size();
    }

    const Item &ScriptContext::GetItem(const string &name) const
    {
        auto item = FindItem(name);
        if (!item) {
            throw std::logic_error("Unknown identifier '" + name + "'");
        }

        return *item;
    }

    const Item &ScriptContext::GetItem(const std::string &name, unsigned int lineNumber) const {
        auto item = FindItem(name);
        if (!item) {
            throw InterpreterError("Unknown identifier '" + name + "'", lineNumber);
        }

        return *item;
    }

    std::shared_ptr<Item> ScriptContext::FindItem(const std::string &name) {
        if (!localScopes.empty()) {
            auto item = localScopes.back()->FindItem(name);
            if (item)
                return item;
        }

        return FindGlobalItem(name);
    }

    std::shared_ptr<const Item> ScriptContext::FindItem(const std::string &name) const {
        assert(!localScopes.empty());

        auto item = localScopes.back()->FindItem(name);
        if (item)
            return item;

        return FindGlobalItem(name);
    }

    void ScriptContext::AddLocalItem(const std::string& name, std::shared_ptr<Item> item) {
        assert(!localScopes.empty());
        localScopes.back()->AddItem(name, item);
    }

    //void ScriptContext::AddItem(const std::string& name, std::shared_ptr<Item> item) {
    //    if (localScopes.empty()) {
    //        globalScopes.back()->AddItem(name, item);
    //    } else{
    //        localScopes.back()->AddItem(name, item);
    //    }
    //}

    void ScriptContext::AddItem(std::shared_ptr<Item> item) {
        assert(!localScopes.empty());
        localScopes.back()->AddItem(item);
    }

    void ScriptContext::PushReturnValue(std::shared_ptr<Item> value) {
        returnValueStack.push_back(value);
    }

    std::shared_ptr<Item> ScriptContext::PopReturnValue() {
        auto result = returnValueStack.back();
        returnValueStack.pop_back();
        return result;
    }

    void ScriptContext::SetCurrentReturnValue(std::shared_ptr<Item> value) {
        assert(!returnValueStack.empty());
        returnValueStack.back() = value;
    }

    void ScriptContext::SetFlag(unsigned int bitmask) {
        flags |= bitmask;
    }

    unsigned int ScriptContext::GetFlags() const {
        return flags;
    }

    bool ScriptContext::IsFlagSet(unsigned int bitmask) {
        return flags & bitmask;
    }

    void ScriptContext::ClearFlag(unsigned int bitmask) {
        flags &= ~bitmask;
    }

    void ScriptContext::ClearFlags() {
        flags = 0;
    }

    std::shared_ptr<ObjectType> ScriptContext::FindObjectType(const std::string &name) {
        if (globalScopes.empty()) {
            return nullptr;
        }

        return globalScopes.back()->FindObjectType(name);
    }

    void ScriptContext::AddObjectType(const std::string &name, std::shared_ptr<ObjectType> type) {
        assert(!globalScopes.empty());

        if (HasObjectType(name)) {
            throw std::logic_error("Unable to add new object type, type name is ambiguous '" + name + "'");
        }

        globalScopes.back()->AddObjectType(name, type);
    }

    bool ScriptContext::HasObjectType(const std::string &name) const {
        if (globalScopes.empty())
            return false;

        return globalScopes.back()->HasObjectType(name, true);
    }

    void ScriptContext::AddGlobalItem(const std::string &name, std::shared_ptr<Item> item) {
        if (globalScopes.empty()) {
            throw std::logic_error("Unable to add global item, no global scope available");
        }

        if (HasGlobalItem(name)) {
            throw std::logic_error("Unable to add global item, ambiguous item  name '" + name + "'");
        }
        globalScopes.back()->AddItem(name, item);
    }

    void ScriptContext::PushGlobalScope(std::shared_ptr<Scope> scope)
    {
        if (!scope) {
            scope = make_shared<Scope>();
        }

        if(!globalScopes.empty()) {
            scope->parent = globalScopes.back();
        }

        globalScopes.push_back(scope);
    }

    void ScriptContext::PopGlobalScope()
    {
        assert(!globalScopes.empty());
        globalScopes.pop_back();
    }

    void ScriptContext::ClearGlobalScopes()
    {
        globalScopes.clear();
    }

    void ScriptContext::PushFunctionScope(std::shared_ptr<Scope> scope)
    {
        if (!scope) {
            scope = make_shared<Scope>();
        }

        localScopes.push_back(scope);
        #ifdef DEBUG_OUTPUT
        std::cout << "added new function scope (" << localScopes.size() << " scopes in total now)" << std::endl;
        #endif
    }

    void ScriptContext::PushCodeBlockScope(std::shared_ptr<Scope> scope)
    {
        if (!scope) {
            scope = make_shared<Scope>();
        }

        if(!localScopes.empty()) {
            scope->parent = localScopes.back();
        }
        localScopes.push_back(scope);
        #ifdef DEBUG_OUTPUT
        std::cout << "added new code block scope (" << localScopes.size() << " scopes in total now)" << std::endl;
        #endif
    }

    void ScriptContext::PopLocalScope()
    {
        assert(!localScopes.empty());
        localScopes.back()->parent.reset();
        localScopes.pop_back();
        #ifdef DEBUG_OUTPUT
        std::cout << "removed local scope (" << localScopes.size() << " scopes left)" << std::endl;
        #endif
    }

    void ScriptContext::ClearLocalScopes()
    {
        localScopes.clear();
    }

    std::shared_ptr<Item> ScriptContext::FindLocalItem(const string &name)
    {
        assert(!localScopes.empty());
        return localScopes.back()->FindItem(name);
    }

    std::shared_ptr<const Item> ScriptContext::FindLocalItem(const string &name) const
    {
        assert(!localScopes.empty());
        return localScopes.back()->FindItem(name);
    }

    std::shared_ptr<Item> ScriptContext::FindGlobalItem(const string &name)
    {
        // Search direction should not make a difference for global scopes, everything must be unique.
        for (auto &scope : globalScopes) {
            auto item = scope->FindItem(name);
            if (item)
                return item;
        }

        return nullptr;
    }

    std::shared_ptr<const Item> ScriptContext::FindGlobalItem(const string &name) const
    {
        // Search direction should not make a difference for global scopes, everything must be unique.
        for (auto &scope : globalScopes) {
            auto item = scope->FindItem(name);
            if (item)
                return item;
        }

        return nullptr;
    }

    bool ScriptContext::HasGlobalItem(const string &name) const
    {
        for (auto &scope : globalScopes) {
            if (scope->HasItem(name))
                return true;
        }

        return false;
    }

    bool ScriptContext::HasLocalItem(const string &name) const
    {
        if (!localScopes.empty()) {
            return localScopes.back()->HasItem(name);
        } else {
            return false;
        }
    }

    void ScriptContext::ClearReturnValues()
    {
        returnValueStack.clear();
    }

    void ScriptContext::PushInstant(std::shared_ptr<Item> instant)
    {
        instantStack.push_back(instant);
    }

    bool ScriptContext::HasInstant() const
    {
        return !instantStack.empty();
    }

    void ScriptContext::PopInstant()
    {
        instantStack.pop_back();
    }

    std::shared_ptr<Item> ScriptContext::PeekInstant()
    {
        assert(!instantStack.empty());
        return instantStack.back();
    }

} // end of namespace
