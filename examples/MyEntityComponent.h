/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// AST Utilities includes
#include <AstuECS.h>

class CMyComponent : public astu::EntityComponent {
public: 
    // Add component data member here...

    CMyComponent() {
        // Intentionally left empty.        
    }

    // Inherited via EntityComponent
    virtual std::shared_ptr<astu::EntityComponent> Clone() override {
        // Create copy using copy-constructor.
        return std::make_shared<CMyComponent>(*this);
    }    
};