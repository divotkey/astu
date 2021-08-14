/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/ICharStream.h"

// C++ Standard Library includes
#include <fstream>
#include <string>

namespace astu {

    class FileCharStream : public ICharStream {
    public:

        /**
         * Constructor.
         */
        FileCharStream();

        /** Destructor. */
        ~FileCharStream();

        /**
         * Opens a file for reading.
         * 
         * @param filename  the name of the file
         */
        void Open(const std::string& filename);

        /**
         * Closes the currently open file.
         */
        void Close();

        // Inherited via IChar Stream
		virtual bool isEos() const override;
		virtual char nextChar() override; 

    private:
        /** The input file stream used to read the file. */
        std::ifstream ifs;
    };

} // end of namespace