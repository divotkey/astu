/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Util/MessageBox.h"

// C++ Standard Library includes.
#include <iostream>

using namespace std;

namespace astu {

    void MessageBox::ShowErrorMessage(const std::string& message, const std::string& title)
    {
        cout << message << endl;
    }

} // end of namespace