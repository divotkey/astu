#include "ScriptContext.h"
#include "InterpreterError.h"
#include "Scope.h"

#include <cassert>

namespace velox {

    const unsigned int ScriptContext::RETURN_EXECUTED_FLAG = 1 << 0;

    void ScriptContext::PushScope(std::shared_ptr<Scope> scope) {
        if (!scope) {
            scope = std::make_shared<Scope>();
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
        for (auto &scope : scopes) {
            auto item = scope->FindItem(name);
            if (item)
                return *item;
        }

        throw InterpreterError("Unknown identifier '" + name + "'");
    }

    const Item &ScriptContext::GetItem(const std::string &name) const {
        for (auto &scope : scopes) {
            auto item = scope->FindItem(name);
            if (item)
                return *item;
        }

        throw InterpreterError("Unknown identifier '" + name + "'");
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

    void ScriptContext::AddItem(const std::string &name, std::shared_ptr<Item> item) {
        scopes.front()->AddItem(name, item);
    }

    void ScriptContext::Clear() {
        scopes.clear();
        returnValueStack.clear();
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

}
