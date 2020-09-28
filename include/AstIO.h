/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 * 
 *----------------------------------------------------------------------------
 * Meanings of ASTU: Slightly melted butter (Hinduism / Religious Law), 
 * Let it be / Be it so (Sanskrit), City (Ancient Greek)
 *----------------------------------------------------------------------------
 */

#pragma once

#include <string>

namespace astu {

    /**
     * Removes all remaining characters from standard input until new line is detected.
     * 
     * The newline character is removed as well by this function.
     */
    void SkipLine();

    /**
     * Queries a string from the standard input stream.
     * 
     * @param text   preceding text printed before the use input is collected
     * @return the string entered by the user
     */
    std::string QueryString(const std::string & text);

    /**
     * Queries an integer value from the standard input stream.
     * 
     * This function ensures that no additional characters entered by the user
     * remain in the input stream.
     * 
     * @param text   preceding text printed before the use input is collected
     * @return the integer value entered by the user
     */
    int QueryInt(const std::string & text);

}