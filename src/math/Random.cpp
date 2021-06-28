/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes.
#include "Random.h"

// C++ Standard Library includes
#include <limits>

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

    double Random::NextDouble(double minValue, double maxValue)
    {
        return minValue + NextDouble() * (maxValue - minValue);
    }

    float Random::NextFloat()
    {
        return floatDist(mt);
    }

    float Random::NextFloat(float minValue, float maxValue)
    {
        return minValue + NextFloat() * (maxValue - minValue);
    }

    int Random::NextInt()
    {
        return intDist(mt);
    }

    int Random::NextInt(int minValue, int maxValue)
    {
        return minValue + static_cast<int>(NextDouble() * (maxValue - minValue));
    }

    void Random::SetSeed(unsigned int value)
    {
        mt.seed(static_cast<uint_fast32_t>(value));
    }

}