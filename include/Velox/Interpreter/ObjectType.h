#pragma once

#include <memory>
#include <vector>
#include <map>
#include <string>

namespace velox {

    class Item;
    class ScriptContext;
    class InterpreterFunctionDefinition;

    class ObjectType {
    public:

        virtual ~ObjectType() {}

        virtual std::shared_ptr<Item> CreateObject(ScriptContext &sc);

        void AddItem(const std::string& name, std::shared_ptr<Item> item);
        bool HasItem(const std::string& name) const;

    private:
        std::map<std::string, std::shared_ptr<Item>> items;
    };

}



