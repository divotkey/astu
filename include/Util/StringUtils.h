/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <string>

namespace astu {

    /**
     * A collection to Utilities related to strings.
     *
	 * @ingroup misc_group
     */
    class StringUtils {
    public:

        // No instances of this class allowed.
        StringUtils() = delete;
        StringUtils(const StringUtils&) = delete;
        StringUtils(StringUtils&&) = delete;

        /**
         * Extracts the file extension from a given filename.
         *
         * The file name can include the path to the file.
         *
         * @param filename  the name of the file to extract
         * @param keepPeriod    whether to include the '.' character
         * @return the file extension or an empty string
         */
        static std::string ExtractFileExtension(const std::string & filename, bool keepPeriod = true);

        /**
         * Removes file extensions from a given filename.
         *
         * @param filename  the file name from with to remove the extension
         * @return the filename without an extension
         */
        static std::string StripFileExtension(const std::string & filename);

    };

} // end of namespace


