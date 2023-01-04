/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Math/Vector2.h"

// C++ Standard library includes
#include <iostream>

namespace astu {

    /**
     * An 2-tuple, often used as two-dimensional index.
     *
     * @ingroup math_group
     */
    template<typename T>
    class Tuple2 {
    public:

        /** Zero tuple to be used as convenient constant. */
        static const Tuple2<T> Zero;

        /** The first element of this 2-tuple. */
        T x;

        /** The second element of this 2-tuple. */
        T y;

        /**
         * Constructor that initializes all components with 0.
         */
        Tuple2() : x(0), y(0)
        {
            // Intentionally left empty.
        }

        /**
         * Constructor.
         *
         * @param x the x-element of the tuple
         * @param y the y-element of the tuple
         */
        Tuple2(T x, T y) : x(x), y(y)
        {
            // Intentionally left empty.
        }

        /**
         * Sets this tuple to the elements of another tuple.
         *
         * @param o the other tuple which element to use
         * @return reference to this tuple for method chaining
         */
        Tuple2 & Set(const Tuple2<T> & o) {
            x = o.x;
            y = o.y;
            return *this;
        }

        /**
         * Sets the x and y elements of this tuple.
         *
         * @param inX  the x-elements of the tuple
         * @param inY  the y-elements of the tuple
         * @return reference to this tuple for method chaining
         */
        Tuple2 & Set(T inX, T inY) {
            x = inX;
            y = inY;
            return *this;
        }

        /**
         * Binary equality operator comparing two tuples.
         *
         * @param rhs   the right hand side tuple
         * @return `true` if the specified tuple is equal to this tuple
         */
        bool operator==(const Tuple2<T> &rhs) const {
            return x == rhs.x && y == rhs.y;
        }

        /**
         * Binary non-equality operator comparing two tuples.
         *
         * @param rhs   the right hand side tuple
         * @return `true` if the specified tuple is not equal to this tuple
         */
        bool operator!=(const Vector2<T> &rhs) const {
            return !(*this == rhs);
        }

    };

    template<typename T>
    inline std::ostream& operator<<(std::ostream& os, const Tuple2<T> &t) {
        os << '(' << t.x << ", " << t.y << ')';
        return os;
    }

    /**
     * Convenient type alias for astu::Tuple2 template using integer as data type.
     */
    using Tuple2i = astu::Tuple2<int>;

    /**
     * Convenient type alias for astu::Tuple2 template using size_t as data type.
     */
    using Tuple2s = astu::Tuple2<size_t>;

    /**
     * Defines the Zero-Constant für Tuple2 templates.
     * (For some reasons, MS C++ compiler does not work with template inline
     * keyword.)
     *
     * @param tparam    the numerical type of the vector
     */
    template <typename T>
    Tuple2<T> const Tuple2<T>::Zero = Tuple2<T>(0, 0);


} // end of namespace
