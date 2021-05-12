/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

class Resource {
public:

    /** Virtual destructor. */
    virtual ~Resource();

    /**
     * Returns whether this resource is currently loaded.
     * 
     * @return `true` if this resource is loaded
     */
    virtual bool IsLoaded() = 0;

};