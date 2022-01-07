/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "FileCharStream.h"

// C++ Standard Library includes
#include <stdexcept>
#include <fstream>

using namespace std;

namespace astu {

    FileCharStream::FileCharStream()
    {
        // Intentionally left empty.
    }

    FileCharStream::~FileCharStream()
    {
        if (ifs) {
            ifs.close();
        }        
    }

    void FileCharStream::Open(const std::string& filename)
    {
        ifs.open(filename, ios::in);

        if (!ifs) {
            throw std::runtime_error(std::string("Unable to open file '") 
				+ filename + "' for reading.");
        }
    }

    void FileCharStream::Close()
    {
        ifs.close();
    }

    bool FileCharStream::FileCharStream::isEos() const
    {
        return !ifs.good();
    }

    char FileCharStream::FileCharStream::nextChar()
    {
        char ch;
        ifs.get(ch);
        return ch;
    }

}