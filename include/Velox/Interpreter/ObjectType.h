// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// C++ Standard Library includes
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

        /** Virtual destructor. */
        virtual ~ObjectType() {}

        virtual std::shared_ptr<Item> CreateObject(ScriptContext &sc);

        void AddItem(const std::string& name, std::shared_ptr<Item> item);
        bool HasItem(const std::string& name) const;

    private:
        std::map<std::string, std::shared_ptr<Item>> items;
    };

}



