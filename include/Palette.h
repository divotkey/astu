/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <vector>
#include "Color.h"

namespace astu {
    
    /**
     * A Palette represents a set of colors used to create color transitions.
     * 
     * @ingroup gfx_group
     */
    class Palette {
    public:

        /**
         * Constructor.
         * 
         * @param start the start color of this palette
         * @param end   the end color of thsi palette
         */
        Palette(const Color4d & start = WebColors::Black, const Color4d & end = WebColors::White);

        /**
         * Sets the start color of this palette.
         * 
         * @param c the start color
         * @returns a reference to this palette for method chaining
         */
        Palette& SetStartColor(const Color4d & c);

        /**
         * Returns the start color of this palette.
         * 
         * @return the start color
         */
        Color4d GetStartColor() const;

        /**
         * Sets the end color of this palette.
         * 
         * @param c the start color
         * @returns a reference to this palette for method chaining
         */
        Palette& SetEndColor(const Color4d & c);

        /**
         * Returns the end color of this palette.
         * 
         * @return the end color
         */
        Color4d GetEndColor() const;

        /**
         * Adds a color to this palette.
         * 
         * @param c the color to add
         * @param p the position within the palette [0, 1]
         * @throws std::out_of_range in case the position is out of range
         */
        Palette& AddColor(const Color4d & c, double p);

        /**
         * Extracts a color from this palette.
         * 
         * The specified position gets clamped to the range [0, 1].
         * 
         * @param pos   the the position within this palette [0, 1]
         * @return the requested color
         */
        Color4d GetColor(double pos) const;

        /**
         * Returns the number of colors in this palette.
         * 
         * @return number of color including start end end color
         */
        size_t NumColors() const {
            return size();
        }

        /**
         * Returns the number of colors in this palette.
         * 
         * @return number of color including start end end color
         */
        size_t size() const;

        /**
         * Returns the color with the specified index.
         * 
         * @param idx   the index of the color to retrieve
         * @return the requested color
         * @throws std::out_of_range in case the index is invalid
         */
        const Color4d & at(size_t idx) const;

		/**
		 * Subscript operator for this palette.
         * 		 *
		 * @param idx	index of the color to be retrieved
		 * @return the requested color
		 */
		const Color4d & operator[](size_t idx) const;

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
            Color4d color;

            /** The position within the palette within the range [0, 1]. */
            double pos;


            /**
             * Constructor
             * 
             * @param c the color of this entry
             * @param p the position within the palette [0, 1]
             */
            Entry(const Color4d & c, double p);

            /**
             * Comparison opeator used to sort entries.
             * 
             * @param rhs   the right-hand-side to compare with
             * @return `true` if this entiry has a lower position
             */
            bool operator<(const Entry & rhs) const;
        };

        /** The default color used when the palette has not enough entries. */
        Color4d defaultColor;

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