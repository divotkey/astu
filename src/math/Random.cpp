/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <limits>
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
        , intDist(0, std::numeric_limits<int>::max())
    {
        std::random_device rd;
        mt.seed(rd());
    }

    double Random::NextDouble()
    {
        return doubleDist(mt);
    }

    int Random::NextInt()
    {
        return intDist(mt);
    }
}