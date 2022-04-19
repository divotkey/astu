// Local includes
#include "ScriptContext.h"
#include "InterpreterError.h"
#include "Scope.h"

// C++ Standard Library includes.
#include <cassert>

namespace velox {

    const unsigned int ScriptContext::RETURN_EXECUTED_FLAG      = 1 << 0;
    const unsigned int ScriptContext::BREAK_EXECUTED_FLAG       = 1 << 1;
    const unsigned int ScriptContext::CONTINUE_EXECUTED_FLAG    = 1 << 2;

    void ScriptContext::PushScope(std::shared_ptr<Scope> scope) {
        if (!scope) {
            scope = std::make_shared<Scope>(false);
        }
        scopes.push_front(scope);
    }

    void ScriptContext::PopScope() {
        scopes.pop_front();
    }

    Scope &ScriptContext::GetCurrentScope() {
        assert(!scopes.empty());
        return *scopes.front();
    }

    const Scope &ScriptContext::GetCurrentScope() const {
        assert(!scopes.empty());
        return *scopes.front();
    }

    bool ScriptContext::HasItem(const std::string &name) const {
        for (auto &scope : scopes) {
            if (scope->HasItem(name))
                return true;
        }

        return false;
    }

    Item &ScriptContext::GetItem(const std::string &name) {
        auto item = FindItem(name);
        if (!item) {
            throw InterpreterError("Unknown identifier '" + name + "'");
        }

        return *item;
    }

    const Item &ScriptContext::GetItem(const std::string &name) const {
        auto item = FindItem(name);
        if (!item) {
            throw InterpreterError("Unknown identifier '" + name + "'");
        }

        return *item;
    }

    std::shared_ptr<Item> ScriptContext::FindItem(const std::string &name) {
        for (auto &scope : scopes) {
            auto item = scope->FindItem(name);
            if (item)
                return item;
        }

        return nullptr;
    }

    std::shared_ptr<const Item> ScriptContext::FindItem(const std::string &name) const {
        for (auto &scope : scopes) {
            auto item = scope->FindItem(name);
            if (item)
                return item;
        }

        return nullptr;
    }

    std::shared_ptr<Item> ScriptContext::FindLocalItem(const std::string &name) {

        for (auto &scope : scopes) {
            auto item = scope->FindItem(name);
            if (item)
                return item;

            if (scope->IsLocalBorder())
                break;
        }

        return nullptr;
    }

    void ScriptContext::AddItem(const std::string &name, std::shared_ptr<Item> item) {
        scopes.front()->AddItem(name, item);
    }

    void ScriptContext::Clear() {
        scopes.clear();
        returnValueStack.clear();
        objectTypes.clear();
        flags = 0;
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
        returnValueStack.back() = value;
    }

    void ScriptContext::SetFlag(unsigned int bitmask) {
        flags |= bitmask;
    }

    unsigned int ScriptContext::GetFlags() const {
        return flags;
    }

    bool ScriptContext::IsSet(unsigned int bitmask) {
        return flags & bitmask;
    }

    void ScriptContext::ClearFlag(unsigned int bitmask) {
        flags &= ~bitmask;
    }

    void ScriptContext::ClearFlags() {
        flags = 0;
    }

    std::shared_ptr<ObjectType> ScriptContext::FindObjectType(const std::string &name) {
        auto it = objectTypes.find(name);
        if (it == objectTypes.end()) {
            return nullptr;
        }
        return it->second;
    }

    void ScriptContext::AddObjectType(const std::string &name, std::shared_ptr<ObjectType> type) {
        assert(!HasObjectType(name));
        objectTypes[name] = type;
    }

    bool ScriptContext::HasObjectType(const std::string &name) const {
        return objectTypes.find(name) != objectTypes.end();
    }

    std::shared_ptr<Item> ScriptContext::FindGlobalItem(const std::string &name) {
        return scopes.back()->FindItem(name);
    }

    void ScriptContext::AddGlobalItem(const std::string &name, std::shared_ptr<Item> item) {
        scopes.back()->AddItem(name, item);
    }

}
