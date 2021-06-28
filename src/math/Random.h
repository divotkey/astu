/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
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
         * Returns a random number within the specified range.
         * This function will generate a random number using a 
         * pseudo random number generator.
         * 
         * The generated numbers are of type `double` and lie within the interval
         * of [minValue, maxValue).
         * 
         * *Note:* the notation for the interval *[minValue, maxValue)* means
         * that `minValue` is included but `maxValue` is not.
         * 
         * @param minValue  the minimum value
         * @param maxValue  the minimum value
         * @return the new random number
         */
        double NextDouble(double minValue, double maxValue);

        /**
         * Returns a random number within the range [0, 1).
         * 
         * @return the next random number.
         */
        float NextFloat();

        /**
         * Returns a random number within the specified range.
         * This function will generate a random number using a 
         * pseudo random number generator.
         * 
         * The generated numbers are of type `float` and lie within the interval
         * of [minValue, maxValue).
         * 
         * *Note:* the notation for the interval *[minValue, maxValue)* means
         * that `minValue` is included but `maxValue` is not.
         * 
         * @param minValue  the minimum value
         * @param maxValue  the minimum value
         * @return the new random number
         */
        float NextFloat(float minValue, float maxValue);

        /**
         * Returns a random integer number within the range [0, INT_MAX].
         * 
         * @return the next random integer number.
         */
        int NextInt();

        /**
         * Returns a random number within the specified range.
         * This function will generate a random number using a 
         * pseudo random number generator.
         * 
         * The generated numbers are of type `int` and lie within the interval
         * of [minValue, maxValue).
         * 
         * *Note:* the notation for the interval *[minValue, maxValue)* means
         * that `minValue` is included but `maxValue` is not.
         * 
         * @param minValue  the minimum value
         * @param maxValue  the minimum value
         * @return the new random number
         */
        int NextInt(int minValue, int maxValue);

        void SetSeed(unsigned int value);

    private:
        /** The one and only instance of this singleton. */
        static std::unique_ptr<Random> theInstance;

        /** The random number generator used to generate pseudo-random numbers. */
        std::mt19937 mt;

        /** The uniform distribution used to create random doubles. */
        std::uniform_real_distribution<double> doubleDist;

        /** The uniform distribution used to create random floats. */
        std::uniform_real_distribution<float> floatDist;

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