/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <iostream>
#include "AstIO.h"

namespace astu {

    /** Used skip input lines etc. */
    static std::string tmpString;

    void SkipLine() {
        getline(std::cin, tmpString);
        tmpString.clear();
    }

    std::string QueryString(const std::string & text)
    {
        if (!text.empty()) {
            std::cout << text << ' ';
        }
        std::string result;
        getline(std::cin, result);
        return result;
    }

    int QueryInt(const std::string & text)
    {
        if (!text.empty()) {
            std::cout << text << ' ';
        }
        int result;
        std::cin >> result;
        SkipLine();
        return result;
    }

} // end of namespace