/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <vector>
#include "Color.h"

namespace astu {
    
    class Palette {
    public:

        /**
         * Constructor.
         * 
         * @param start the start color of this palette
         * @param end   the end color of thsi palette
         */
        Palette(const Color & start = WebColors::Black, const Color & end = WebColors::Black);

        /**
         * Sets the start color of this palette.
         * 
         * @param c the start color
         */
        void SetStartColor(const Color & c);

        /**
         * Returns the start color of this palette.
         * 
         * @return the start color
         */
        Color GetStartColor() const;

        /**
         * Sets the end color of this palette.
         * 
         * @param c the start color
         */
        void SetEndColor(const Color & c);

        /**
         * Returns the end color of this palette.
         * 
         * @return the end color
         */
        Color GetEndColor() const;

        /**
         * Adds a color to this palette.
         * 
         * @param c the color to add
         * @param p the position within the palette [0, 1]
         * @throws std::out_of_range in case the position is out of range
         */
        void AddColor(const Color & c, double p);

        Color GetColor(double pos);

    private:

        /**
         * An inner class which functions as decorator for for color entries.
         * 
         * Decorators are a design pattern which can add additional members
         * variables and functionality to other classes.
         */
        class Entry {
        public:
            /** The color of this palette entry. */
            Color color;

            /** The position within the palette within the range [0, 1]. */
            double pos;


            /**
             * Constructor
             * 
             * @param c the color of this entry
             * @param p the position within the palette [0, 1]
             */
            Entry(const Color & c, double p);

            /**
             * Comparison opeator used to sort entries.
             * 
             * @param rhs   the right-hand-side to compare with
             * @return `true` if this entiry has a lower position
             */
            bool operator<(const Entry & rhs) const;
        };

        /** The default color used when the palette has not enough entries. */
        Color defaultColor;

        /** The color entries of this palette. */
        std::vector<Entry> entries;

        /**
         * Finds the two entries which surround the specified position.
         *
         * @param pos   the position to look for the surrounding entries
         * @param e1    receives the address of the entry which lies below the position
         * @param e2    receives the address of the entry which lies above the position
         */
        void FindBoundaries(double pos, const Entry *&e1, const Entry *&e2) const;
    };

}