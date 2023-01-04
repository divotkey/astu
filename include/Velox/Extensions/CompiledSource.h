/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterScript.h"
#include "Velox/Interpreter/Interpreter.h"

// C++ Standard Library includes
#include <memory>

namespace astu {

    /**
     * Source code, that has been tokenized and the raw data have been compiled as C++ class member.
     */
    class CompiledSource {
    public:

        /**
         * Virtual destructor.
         */
        virtual ~CompiledSource() = default;

        /**
         * Returns the parsed source
         *
         * @return the parsed source code
         * @throws ParserError in case the source could not be parsed correctly
         */
        velox::InterpreterScript &GetParsedSource() const;

        /**
         * Executes the source with the specified interpreter.
         *
         * @param interpreter   the interpreter used to execute the script
         */
        void Execute(velox::Interpreter &interpreter) const;

    protected:

        /**
         * Returns the raw data.
         *
         * @return the raw data
         */
        virtual const unsigned char *GetData() const = 0;

        /**
         * Returns the size of the raw data.
         *
         * @return  the size of the raw data in bytes
         */
        virtual size_t GetDataSize() const = 0;

    private:
        /** The parsed script. */
        mutable std::unique_ptr<velox::InterpreterScript> parsedSource;
    };

} // end of namespace

