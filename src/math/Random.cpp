/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include "Random.h"

namespace astu {

    std::unique_ptr<Random> Random::theInstance;

    Random & Random::GetInstance()
    {
        if (theInstance == nullptr) {
            theInstance = std::unique_ptr<Random>(new Random());
        }

        return *theInstance;
    }

    Random::Random()
        : doubleDist(0, 1)
    {
        std::random_device rd;
        mt.seed(rd());
    }

    double Random::NextDouble()
    {
        return doubleDist(mt);
    }
}