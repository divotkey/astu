/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Tuple2.h"
#include "Graphics/Color.h"

// C++ Standard Library includes
#include <vector>
#include <string>
#include <cstdint>

namespace astu {

    /**
     * An implementation of the software design pattern Memento.
     *
     * @ingroup misc_group
     */
    class Memento {
    public:

        /**
         * Constructor.
         */
        Memento();

        /**
         * Constructor.
         *
         * @param data	the data of this memento
         */
        Memento(const std::vector<unsigned char> &data);

        /**
         * Constructor.
         *
         * @param data	the data of this memento
         * @param len   the length of the data array
         */
        Memento(const unsigned char data[], size_t len);

        /**
         * Insertion operator used to store vectors of unsigned characters.
         *
         * @param data  the vector of character data
         * @return reference tho this memento for method chaining
         */
        Memento &operator<<(const std::vector<unsigned char> &data);

        /**
         * Insertion operator for 64-bit integer values.
         *
         * @param value the 64-bit integer value to insert
         * @return reference tho this memento for method chaining
         */
        Memento &operator<<(const int64_t &value);

        /**
         * Extraction operator for 64-bit integer values.
         *
         * @param value the 64-bit integer to extract
         * @return reference tho this memento for method chaining
         */
        const Memento &operator>>(int64_t &value) const;

        /**
         * Insertion operator for unsigned 64-bit integer values.
         *
         * @param value the unsigned 64-bit integer value to insert
         * @return reference tho this memento for method chaining
         */
        Memento &operator<<(const uint64_t &value);

        /**
         * Extraction operator for unsigned 64-bit integer values.
         *
         * @param value the unsigned 64-bit integer to extract
         * @return reference tho this memento for method chaining
         */
        const Memento &operator>>(uint64_t &value) const;

        /**
           * Insertion operator for signed integer integer values.
           *
           * @param value the signed integer value to insert
           * @return reference tho this memento for method chaining
           */
        Memento &operator<<(const char &value);

        /**
         * Extraction operator for signed characters.
         *
         * @param value the signed character to extract
         * @return reference tho this memento for method chaining
         */
        const Memento &operator>>(char &value) const;

        /**
         * Insertion operator for signed signed characters.
         *
         * @param value the signed character to insert
         * @return reference tho this memento for method chaining
         */
        Memento &operator<<(const int &value);

        /**
         * Extraction operator for signed integer values.
         *
         * @param value the signed integer to extract
         * @return reference tho this memento for method chaining
         */
        const Memento &operator>>(int &value) const;

        /**
         * Insertion operator for unsigned integer integer values.
         *
         * @param value the unsigned value to insert
         * @return reference tho this memento for method chaining
         */
        Memento &operator<<(const unsigned int &value);

        /**
         * Extraction operator for unsigned integer values.
         *
         * @param value the unsigned integer to extract
         * @return reference tho this memento for method chaining
         */
        const Memento &operator>>(unsigned int &value) const;

        /**
         * Insertion operator for floating point values with single precision.
         *
         * @param value floating point value to insert
         * @return reference tho this memento for method chaining
         */
        Memento &operator<<(const float &value);

        /**
         * Extraction operator for floating point values with single precision.
         *
         * @param value the floating point value to extract
         * @return reference tho this memento for method chaining
         */
        const Memento &operator>>(float &value) const;

        /**
         * Insertion operator for floating point values with double precision.
         *
         * @param value floating point value to insert
         * @return reference tho this memento for method chaining
         */
        Memento &operator<<(const double &value);

        /**
         * Extraction operator for floating point values with double precision.
         *
         * @param value the floating point value to extract
         * @return reference tho this memento for method chaining
         */
        const Memento &operator>>(double &value) const;

        /**
         * Insertion operator for string.
         *
         * @param value floating point value to insert
         * @return reference tho this memento for method chaining
         */
        Memento &operator<<(const std::string &s);

        /**
         * Extraction operator for strings.
         *
         * @param s the string to extract
         * @return reference tho this memento for method chaining
         */
        const Memento &operator>>(std::string &s) const;

        /**
         * Insertion operator for two dimensional vector objects with single precision.
         *
         * @param v the vector to insert
         * @return reference tho this memento for method chaining
         */
        Memento &operator<<(const Vector2f &v);

        /**
         * Extraction operator for two dimensional vector objects with single precision.
         *
         * @param v the vector to extract
         * @return reference tho this memento for method chaining
         */
        const Memento &operator>>(Vector2f &v) const;

        /**
         * Insertion operator for two dimensional vector objects with double precision.
         *
         * @param v the vector to insert
         * @return reference tho this memento for method chaining
         */
        Memento &operator<<(const Vector2d &v);

        /**
         * Extraction operator for two-dimensional vector of various types.
         *
         * @tparam T    the type parameter for the vector
         * @param v     the vector to extract
         * @return reference tho this memento for method chaining
         */
        template<typename T>
        const Memento &operator>>(Vector2<T> &v) const
        {
            *this >> v.x >> v.y;
            return *this;

        }

        /**
         * Insertion operator for two-dimensional vectors of various types.
         *
         * @tparam T    the type parameter for the vector
         * @param v     the vector to insert
         * @return reference tho this memento for method chaining
         */
        template<typename T>
        Memento &operator<<(const Vector2<T> &v)
        {
            *this << v.x << v.y;
            return *this;
        }

        /**
         * Extraction operator for three-dimensional vector of various types.
         *
         * @tparam T    the type parameter for the vector
         * @param v     the vector to extract
         * @return reference tho this memento for method chaining
         */
        template<typename T>
        const Memento &operator>>(Vector3<T> &v) const
        {
            *this >> v.x >> v.y >> v.z;
            return *this;

        }

        /**
         * Insertion operator for three-dimensional vectors of various types.
         *
         * @tparam T    the type parameter for the vector
         * @param v     the vector to insert
         * @return reference tho this memento for method chaining
         */
        template<typename T>
        Memento &operator<<(const Vector3<T> &v)
        {
            *this << v.x << v.y << v.z;
            return *this;
        }

        /**
         * Insertion operator for colors of various types.
         *
         * @tparam T    the type parameter for the color
         * @param c     the color to insert
         * @return reference tho this memento for method chaining
         */
        template<typename T>
        Memento &operator<<(const Color<T> &c)
        {
            *this << c.r << c.g << c.b << c.a;
            return *this;
        }

        /**
         * Extraction operator for colors of various types.
         *
         * @tparam T    the type parameter for the colors
         * @param c     the colors to extract
         * @return reference tho this memento for method chaining
         */
        template<typename T>
        const Memento &operator>>(Color<T> &c) const
        {
            *this >> c.r >> c.g >> c.b >> c.a;
            return *this;
        }

        /**
         * Insertion operator for tuples of various types.
         *
         * @tparam T    the type parameter for the tuple
         * @param t     the tuple to insert
         * @return reference tho this memento for method chaining
         */
        template<typename T>
        Memento &operator<<(const Tuple2<T> &t)
        {
            *this << t.x << t.y;
            return *this;
        }

        /**
         * Extraction operator for tuples of various types.
         *
         * @tparam T    the type parameter for the tuple
         * @param t     the tuple to extract
         * @return reference tho this memento for method chaining
         */
        template<typename T>
        const Memento &operator>>(Tuple2<T> &t) const
        {
            *this >> t.x >> t.y;
            return *this;
        }

        /**
         * Stores raw bytes in this memento
         *
         * @param bytes	pointer to the byte array
         * @param the number of bytes to write
         */
        Memento &Write(const unsigned char *bytes, size_t count);

        /**
         * Reads raw bytes from this memento.
         *
         * @param bytes	pointer to the byte array
         * @param the number of bytes to read
         * @return reference to this memento for method chaining
         */
        const Memento &Read(unsigned char *bytes, size_t count) const;

        /**
         * Rewinds this memento.
         *
         * The reading position is set to zero.
         */
        const Memento &Rewind() const;

        /**
         * Sets this buffer's mark at its position.
         *
         * @return reference to this memento for method chaining
         */
        const Memento& Mark() const;

        /**
         * Resets this buffer's position to the previously-marked position.
         *
         * @return reference to this memento for method chaining
         */
        const Memento& Reset() const;

        /**
         * Releases all data of this memento.
         *
         * @return reference to this memento for method chaining
         */
        Memento &Clear();

        /**
         * Returns the raw data stored in this memento.
         *
         * @return this memento's data
         */
        const std::vector<unsigned char> &GetData() const
        {
            return data;
        }

    private:
        /** The data of this memento. */
        std::vector<unsigned char> data;

        /** The current reading position. */
        mutable size_t pos;

        /** The marked position. */
        mutable size_t mark;
    };

} // end of namespace