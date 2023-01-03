/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <algorithm>
#include <iostream>

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
         * If the start point is greater than the end point, the
         * behaviour is undefined.
         * 
         * @param x0    the start point
         * @param x1    the end point
         * @return reference to this segment for method chaining
         */
        Segment1& Set(T x0, T x1) {
            assert(x0 < x1);
            x0 = x0;
            x1 = x1;
            return *this;
        }

        /**
         * Sets the start point and end point to the specified value.
         *  
         * @param x the new start and end point
         * @return reference to this segment for method chaining
         */
        Segment1& Set(T x) {
            x0 = x1 = x;
            return *this;
        }

        /**
         * Sets the start point and end point of this segment.
         * 
         * This setter will swap start and end point in case
         * the start point is greater than the end point.
         * 
         * @param _x0    the start point
         * @param _x1    the end point
         * @return reference to this segment for method chaining
         */
        Segment1& SetSafe(T _x0, T _x1) {
            x0 = std::min(_x0, _x1);
            x1 = std::max(_x0, _x1);
            return *this;
        }

        /**
         * Returns the length of this segment.
         * 
         * @return the length
         */
        T Length() {
            return x1 - x0;
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
        bool IsIntersecting(const Segment1<T>& other) {
            return x1 >= other.x0 && other.x1 >= x0;
        }

        /**
         * Calculates the overlap of this segment with the specified segment.
         * 
         * @param other the other segment
         * @return the overlap distance or zero there is no overlap
         */
        T CalcOverlap(const Segment1<T>& other) {
            return std::max(
                static_cast<T>(0), 
                std::min(x1, other.x1) - std::max(x0, other.x0)
                );
        }

        /**
         * Calculates the separation distance with respect to the other segment.
         * 
         * The returned distance is the distance this segment must be moved 
         * forward in order to separate the two segments.
         * 
         * @param other the other segment
         * @return the separation distance
         */
        T CalcSeparationDistance(const Segment1<T>& other) {
            return other.x1 - x0;
        }

        /**
         * Calculates the minimum separation distance to the specified segment.
         * 
         * The returned distance will always gerater or equal zero. 
         * 
         * @param other the other segment
         * @return the separation distance
         */
        T CalcMinimumSeparationDistance(const Segment1<T>& other) {
            if (!IsIntersecting(other)) {
                return 0;
            }

            return std::min(std::abs(other.x0 - x1), std::abs(other.x1 - x0));
        }

        /**
         * Translates this segment.
         * 
         * @param delta the delta movement
         * @return reference to this segment for method chaining
         */
        Segment1& Translate(T delta) {
            x0 += delta;
            x1 += delta;
            return *this;
        }

        /**
         * Compound assignment and addition operator.
         *
         * @param delta the delta movement to the segment
         * @return a reference to this vector
         */
       Segment1& operator+=(T delta)
        {
            x0 += delta;
            x1 += delta;
            return *this;
        }        

    private:
        /** The start point of this segment. */
        T x0;

        /** The end point of this segment. */
        T x1;
    };

    /**
     * Type alias for Segment1 template using double as data type.
     * 
     * @ingroup math_group
     */
    using Segment1d = astu::Segment1<double>;    

    /**
     * Type alias for Segment1 template using float as data type.
     * 
     * @ingroup math_group
     */
    using Segment1f = astu::Segment1<float>;    

    template<typename T>
	inline std::ostream& operator<<(std::ostream& os, const Segment1<T> &seg) {
		os << '{' << seg.GetX0() << ", " << seg.GetX1() << '}';
		return os;
	}    

} // end of namespace