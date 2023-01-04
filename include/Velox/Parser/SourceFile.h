/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Source.h"

// C++ Standard Library includes
#include <sstream>
#include <string>

namespace velox {

    class SourceFile : public Source {
    public:

        /**
         * Constructor
         */
        SourceFile() = default;

        /**
         * Constructor
         *
         * @param filePath  the path to the source file
         */
        SourceFile(const std::string& filePath);

        /**
         * Return the path to the source file.
         *
         * @return the path to the source file.
         */
        const std::string& GetFilePath() const;

        /**
         * Resets this source file to be reused.
         *
         * @param filePath  ht path to the source file
         */
        void Reset(const std::string &filePath);

    protected:
        // Inherited via Source
        std::shared_ptr<std::istream> GetStream() override;

    private:
        /** The path to the source file. */
        std::string filePath;
    };

}





