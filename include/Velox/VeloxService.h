/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes.
#include "Service/Service.h"
#include "Velox/FileCharStream.h"

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
         * @param filename  the name of the velox script
         */
        void RunScript(const std::string& filename);

    private:
        /** The parser used to convert script source AST. */
        std::unique_ptr<VeloxParser> parser;

        /** Used to read the input file. */
        std::shared_ptr<FileCharStream> cs;
    };

} // end of namespace