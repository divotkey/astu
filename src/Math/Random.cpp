/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes.
#include "Math/Random.h"

// C++ Standard Library includes
#include <limits>

namespace astu {

    std::unique_ptr<Random> Random::theInstance;

    Random& Random::GetInstance()
    {
        if (theInstance == nullptr) {
            theInstance = std::unique_ptr<Random>(new Random());
        }

        return *theInstance;
    }

    Random::Random()
        : doubleDist(0, 1)
        , floatDist(0, 1)
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

    Vector2f Random::NextVector2f(float length)
    {
        Vector2f result(0, length);
        result.Rotate(NextFloat(0, MathUtils::PI2f));
        return result;
    }

    Vector2d Random::NextVector2d(double length)
    {
        Vector2d result(0, length);
        result.Rotate(NextDouble(0, MathUtils::PI2d));
        return result;
    }

    void Random::SetSeed(unsigned int value)
    {
        mt.seed(static_cast<uint_fast32_t>(value));
    }

} // end of namespace