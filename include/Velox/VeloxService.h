/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes.
#include "Service/Service.h"
#include "Velox/FileCharStream.h"
#include "Velox/StringCharStream.h"

// C++ Standard Library includes.
#include <string>
#include <memory>

namespace astu {

    // Forward declaration.
    class VeloxParser;

    class VeloxService : public virtual Service 
    {
    public:

        /** Constructor. */
        VeloxService();

        /** Virtual destructor. */
        virtual ~VeloxService();

        /**
         * Loads and executes a velox script.
         * 
         * @param filename  the filename including path of the velox script
         */
        void RunScriptFromFile(const std::string& filename);

        /**
         * Loads and executes a velox script.
         * 
         * @param script    the script as string
         */
        void RunScriptFromString(const std::string& script);


    private:
        /** The parser used to convert script source AST. */
        std::unique_ptr<VeloxParser> parser;

        /** Used to read the input file. */
        std::shared_ptr<FileCharStream> fcs;

        /** Used to hold scripts. */
        std::shared_ptr<StringCharStream> scs;
    };

} // end of namespace