#include "ScriptContext.h"
#include "InterpreterError.h"
#include "Scope.h"

namespace velox {

    void ScriptContext::PushScope(std::shared_ptr<Scope> scope) {
        if (!scope) {
            scope = std::make_shared<Scope>();
        }
        scopes.push_front(scope);
    }

    void ScriptContext::PopScope() {
        scopes.pop_front();
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
    }

}
