/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Extensions/ObjectTypeBuilder.h"
#include "Velox/Interpreter/ObjectType.h"


// C++ Standard Library includes
#include <stdexcept>

using namespace velox;
using namespace std;

namespace astu {

    ObjectTypeBuilder::ObjectTypeBuilder()
    {
        Reset();
    }

    ObjectTypeBuilder &ObjectTypeBuilder::TypeName(const std::string &name)
    {
        typeName = name;
        return *this;
    }

    ObjectTypeBuilder &ObjectTypeBuilder::Constructor(std::shared_ptr<velox::Item> constr)
    {
        constructor = constr;
        return *this;
    }

    ObjectTypeBuilder &ObjectTypeBuilder::AddFunction(const std::string &name, std::shared_ptr<velox::Item> func)
    {
        if (HasFunction(name)) {
            throw std::logic_error("Unable to add function to Velox object type, ambiguous function name '" + name + "'");
        }

        functions[name] = func;
        return *this;
    }

    ObjectTypeBuilder &ObjectTypeBuilder::Reset()
    {
        typeName.clear();
        constructor = nullptr;
        functions.clear();
        return *this;
    }

    void ObjectTypeBuilder::Build(velox::Interpreter &interpreter)
    {
        if (typeName.empty()) {
            throw std::logic_error("Unable to build Velox object type, no type name specified");
        }

        if (!constructor) {
            throw std::logic_error("Unable to build Velox object type, no constructor function");
        }

        auto newType = make_shared<ObjectType>();
        newType->AddItem(typeName, constructor);

        for (auto &it : functions) {
            newType->AddItem(it.first, it.second);
        }

        interpreter.AddObjectType(typeName, newType);
        Reset();
    }

    bool ObjectTypeBuilder::HasFunction(const string &name) const
    {
        return functions.find(name) != functions.end();
    }


} // end of namespace
