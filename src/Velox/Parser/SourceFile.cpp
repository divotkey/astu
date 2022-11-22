// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "Velox/Parser/SourceFile.h"

// C++ Standard Library includes
#include <fstream>

using namespace std;

namespace velox {

    SourceFile::SourceFile(const string &inFilePath) : filePath(inFilePath)
    {
        // Intentionally left empty.
    }

    void SourceFile::Reset(const string &inFilePath)
    {
        filePath = inFilePath;
        Source::Reset();
    }

    shared_ptr<istream> SourceFile::GetStream() {
        // Input file stream must be binary to conserve line breaks.
        auto result = make_shared<ifstream>(filePath, ios::in | ios::binary);

        if (!*result) {
            throw std::runtime_error("Unable to open source file '" + filePath + "' for reading.");
        }

        return result;
    }

    const std::string &SourceFile::GetFilePath() const
    {
        return filePath;
    }
}
