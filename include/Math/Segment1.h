/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// C++ Standard Library includes
#include <algorithm>

namespace astu {

    /**
     * A one-dimensional line segment. 
     * 
     * This class is a mathematical utility class to carry out some tests
     * with one-dimensional segments.
     * 
     * @tparam T    the numerical data type used for this segment
     * 
     * @ingroup math_group
     */
    template <typename T>
    class Segment1 {
    public:

        /**
         * Constructor.
         */
        Segment1() : x0(0), x1(0) {
            // Intentionally left empty.
        }

        /**
         * Constructor.
         * 
         * @param x0    the starting point of this segment
         * @param x1    the end point of this segment
         */
        Segment1(T x0, T x1) : x0(x0), x1(x1) {
            // Intentionally left empty.
        }

        /**
         * Sets the start point and end point of this segment.
         * 
         * @param x0    the start point
         * @param x1    the end point
         * @return reference to this segment for method chaining
         */
        Segment1& Set(T x0, T x1) {
            x0 = x0;
            x1 = x1;
            return *this;
        }

        /**
         * Sets the start point and end point of this segment.
         * 
         * @param x0    the start point
         * @param x1    the end point
         * @return reference to this segment for method chaining
         */
        Segment1& SetSafe(T x0, T x1) {
            x0 = std::min(x0, x1);
            x1 = std::max(x0, x1);
            return *this;
        }

        /**
         * Extends this segment to include the specified point.
         * 
         * @param x the point to include
         * @return reference to this segment for method chaining
         */
        Segment1& Extend(T x) {
            if(x < x0) {
                x0 = x;
            } else if (x > x1) {
                x1 = x;
            }

            return *this;
        }

        /**
         * Returns the start point of this segment.
         * 
         * @return the start point
         */
        T GetX0() const {
            return x0;
        }

        /**
         * Returns the end point of this segment.
         * 
         * @return the end point
         */
        T GetX1() const {
            return x1;
        }

        /**
         * Tests whether this segments intersects with the specified segment.
         * 
         * @param other the other segment
         * @return `true` if the two segments intersect
         */
        bool IsIntersecting(const Segment1<T> & other) {
            return x1 >= other.x0 && other.x1 >= x0;
        }

        /**
         * Calculates the overlap of this segment with the specified segment.
         * 
         * @param other the other segment
         * @return the overlap distance or zero there is no overlap
         */
        T CalcOverlap(const Segment1<T> & other) {
            return std::max(
                static_cast<T>(0), 
                std::min(x1, other.x1) - std::max(x0, other.x0)
                );
        }

    private:
        /** The start point of this segment. */
        T x0;

        /** The end point of this segment. */
        T x1;
    };

    /**
     * Type alias for astu::Segment1 template using double as data type.
     */
    using Segment1d = astu::Segment1<double>;    

    /**
     * Type alias for astu::Segment1 template using float as data type.
     */
    using Segment1f = astu::Segment1<float>;    

} // end of namespace