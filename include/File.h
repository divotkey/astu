/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <string>

namespace astu {

    class InputFile final {
    public:

        /**
         * Construct.
         * 
         * @param filename  the filename including the path
         * @throws std::runtime_exception in case the file could not be opened for reading
         */
        InputFile(const std::string & filename);

        /**
         * Reads a certain amount of bytes from the file.
         * 
         * The memory of the specified byte buffer has to be allocated by
         * the caller and must be big enough to hold the maximum number of
         * bytes to read.
         * 
         * @param data      receives the bytes read from the file
         * @param numBytes  the maximum number of bytes to read
         * @return the actual number of bytes read from the file
         * @throws std::runtime_error in case of an I/O error
         */
        size_t Read(unsigned char * data, size_t numBytes);

    private:
    };

}