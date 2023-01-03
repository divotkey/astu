/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "ScriptContext.h"

namespace velox {

    class InterpreterStatement {
    public:

        /**
         * Constructor.
         *
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterStatement(unsigned int lineNumber = 0) : lineNumber (lineNumber) {};

        /** Virtual destructor. */
        virtual ~InterpreterStatement() {}

        /**
         * Executes this statement.
         * @param sc    the script context
         */
        virtual void Execute(ScriptContext &sc) = 0;

        /**
         * Prepares this statement for execution.
         * This method should be called once before calling `Execute`
         *
         * @param sc    the script context
         */
        virtual void Prepare(ScriptContext &sc) {}

        /**
         * Returns the linen number within the source code.
         *
         * @return  the line number of this statement
         */
        unsigned int GetLineNumber() const {
            return lineNumber;
        }

    private:
        /** Information about the location within the source code */
        unsigned int lineNumber;
    };

}


