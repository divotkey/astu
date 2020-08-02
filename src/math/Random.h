/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <memory>
#include <random>

namespace astu {

    class Random {
    public:

        static Random & GetInstance();

        double NextDouble();

    private:
        static std::unique_ptr<Random> theInstance;
        std::mt19937 mt;
        std::uniform_real_distribution<double> doubleDist;

        Random();
		Random(const Random&) = delete;
		Random& operator=(const Random&) = delete;
    };

}