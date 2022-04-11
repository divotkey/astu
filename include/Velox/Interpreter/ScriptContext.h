#pragma once

#include <memory>
#include <string>
#include <deque>
#include <vector>
#include <map>

namespace velox {

    // Forward declaration.
    class Item;
    class Scope;
    class ObjectType;

    class ScriptContext {

    public:
        static const unsigned int RETURN_EXECUTED_FLAG;

        std::shared_ptr<ObjectType> FindObjectType(const std::string& name);
        void AddObjectType(const std::string& name, std::shared_ptr<ObjectType> type);
        bool HasObjectType(const std::string& name) const;

        void SetFlag(unsigned int bitmask);
        void ClearFlag(unsigned int bitmask);
        unsigned int GetFlags() const;
        bool IsSet(unsigned int bitmask);
        void ClearFlags();

        void PushScope(std::shared_ptr<Scope> scope = nullptr);
        void PopScope();
        Scope& GetCurrentScope();
        const Scope& GetCurrentScope() const;

        void PushReturnValue(std::shared_ptr<Item> value = nullptr);
        std::shared_ptr<Item> PopReturnValue();
        void SetCurrentReturnValue(std::shared_ptr<Item> value);


        bool HasItem(const std::string &name) const;
        Item& GetItem(const std::string &name);
        const Item& GetItem(const std::string &name) const;
        std::shared_ptr<Item> FindItem(const std::string &name);
        std::shared_ptr<const Item> FindItem(const std::string &name) const;
        void AddItem(const std::string& name, std::shared_ptr<Item> item);
        void Clear();

    private:
        /** The hierarchy of scopes. */
        std::deque<std::shared_ptr<Scope>> scopes;

        /** Stack of return values, used by functions and return statements. */
        std::vector<std::shared_ptr<Item>> returnValueStack;

        /** Type definitions for custom objects. */
        std::map<std::string, std::shared_ptr<ObjectType>> objectTypes;

        /** Flags to be set and queried during execution of scripts. */
        unsigned int flags;
    };

}


