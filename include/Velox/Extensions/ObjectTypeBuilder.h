/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include <Velox/Interpreter/Item.h>
#include <Velox/Interpreter/Interpreter.h>

// C++ Standard Library includes
#include <string>
#include <memory>
#include <map>

namespace astu {

    class ObjectTypeBuilder {
    public:

        ObjectTypeBuilder();

        ObjectTypeBuilder &TypeName(const std::string &name);
        ObjectTypeBuilder &Constructor(std::shared_ptr<velox::Item> constr);
        ObjectTypeBuilder &AddFunction(const std::string &name, std::shared_ptr<velox::Item> func);
        bool HasFunction(const std::string & name) const;

        ObjectTypeBuilder &Reset();
        void Build(velox::Interpreter &interpreter);

    private:
        std::string typeName;
        std::shared_ptr<velox::Item> constructor;
        std::map<std::string, std::shared_ptr<velox::Item>> functions;
    };

} // end of namespace
