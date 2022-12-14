#pragma once

// C++ Standard Library includes.
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
        /** Constant for the flag which denotes that a return statement has been executed. */
        static const unsigned int RETURN_EXECUTED_FLAG;

        /** Constant for the flag which denotes that a break statement has been executed. */
        static const unsigned int BREAK_EXECUTED_FLAG;

        /** Constant for the flag which denotes that a continue statement has been executed. */
        static const unsigned int CONTINUE_EXECUTED_FLAG;


        /**
         * Searches for an object type.
         *
         * @param name  the name of the object type
         * @return the requested object type or 'nullptr' if no object type with the specified name could be found
         */
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
        std::shared_ptr<Item> FindItem(const std::string &name);
        void AddItem(const std::string& name, std::shared_ptr<Item> item);

        /**
         * Adds an anonymous item to this scope.
         *
         * @param item  the item to add
         */
        void AddItem(std::shared_ptr<Item> item);

        Item& GetItem(const std::string &name);
        const Item& GetItem(const std::string &name) const;

        std::shared_ptr<const Item> FindItem(const std::string &name) const;
        std::shared_ptr<Item> FindLocalItem(const std::string& name);
        std::shared_ptr<Item> FindGlobalItem(const std::string& name);

        void AddGlobalItem(const std::string& name, std::shared_ptr<Item> item);
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


