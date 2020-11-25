/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <memory>
#include <random>

namespace astu {

    /**
     * This singleton simplifies the creation of random numbers.
     */
    class Random final {
    public:

        /**
         * Returns the one and only instance of this class.
         *
         * @return the instance of this class
         */
        static Random & GetInstance();

        /**
         * Returns a random number within the range [0, 1).
         * 
         * @return the next random number.
         */
        double NextDouble();

        /**
         * Returns a random integer number within the range [0, INT_MAX].
         * 
         * @return the next random integer number.
         */
        int NextInt();

    private:
        /** The one and only instance of this singleton. */
        static std::unique_ptr<Random> theInstance;

        /** The random number generator used to generate pseudo-random numbers. */
        std::mt19937 mt;

        /** The uniform distribution used to create random doubles. */
        std::uniform_real_distribution<double> doubleDist;

        /** The uniform distribution used to create random integers. */
        std::uniform_int_distribution<int> intDist;


        /////////////////////////////////////////////////
        /////// Avoid creation of an instance.
        /////////////////////////////////////////////////

        // Private constructor.
        Random();

        // Deleted copy-constructor.
		Random(const Random&) = delete;

        // Deleted assignment operator
		Random& operator=(const Random&) = delete;
    };

}