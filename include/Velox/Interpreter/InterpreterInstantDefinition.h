// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterStatement.h"
#include "Velox/Interpreter/InterpreterFunctionDefinition.h"

// C++ Standard Library includes
#include <string>
#include <vector>
#include <memory>

namespace velox {

    // Forward declaration
    class ObjectType;

    class InterpreterInstantDefinition : public InterpreterStatement {
    public:

        /**
         * Constructor.
         *
         * @param name          the name of the instant
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterInstantDefinition(const std::string & name, unsigned int lineNumber);

        /**
         * Tests whether a function definition with the specified name has already been added.
         *
         * @param name  the name of the function to look for
         * @return `true` in case a function with the specified name has already been added
         */
        bool HasFunction(const std::string& name) const;

        /**
         * Adds the specified function definition to this instant definition.
         * In case a function definition with an identical name has already been added, the behaviour is undefined.
         *
         * @param function  the function to add
         */
        void AddFunction(std::shared_ptr<InterpreterFunctionDefinition> function);

        /**
         * Returns the name of the type of this instant definition.
         *
         * @return this instant's name
         */
        const std::string& GetTypeName() const;

        /**
         * Creates the object type of this instance definition.
         *
         * @return the newly created object type of this instance definition
         */
        std::shared_ptr<ObjectType> CreateObjectType() const;

        // Inherited via InterpreterStatement
        void Execute(ScriptContext &sc) override;
        void Prepare(ScriptContext &sc) override;

    private:
        /** The name of the instant type. */
        std::string typeName;

        /** The member functions of the instant. */
        std::vector<std::shared_ptr<InterpreterFunctionDefinition>> functions;
    };

}



