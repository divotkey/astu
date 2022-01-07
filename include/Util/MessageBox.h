/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes.
#include <string>

namespace astu {

    /**
     * Utility class to show simple messages to the user.
     * 
     * This class is typically used to report error.
     */
    class MessageBox {
    public:

        /**
         * Shows an error message to the user.
         * 
         * @param message   the message to present
         * @param title     the title of the message box window
         */
        static void ShowErrorMessage(const std::string& message, const std::string& title);

    };

} // end of namespace