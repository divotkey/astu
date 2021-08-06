/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */


#pragma once

// Local includes
#include "Math/Vector2.h"
#include "Graphics/Color.h"

// C++ Standard Library includes
#include <vector>
#include <string>
#include <cstdint>

namespace astu {

	class Memento {
	public:

		/**
		 * Constructor.
		 */
		Memento();

		/**
		 * Constructor.
		 * 
		 * @param data	of this memento
		 */
		Memento(const std::vector<unsigned char> & data);

		Memento & operator<<(const std::vector<unsigned char> & data);

		Memento & operator<<(const int64_t & value);
		const Memento& operator>>(int64_t & value) const;

		Memento & operator<<(const int & value);
		const Memento& operator>>(int & value) const;

		Memento & operator<<(const unsigned int & value);
		const Memento& operator>>(unsigned int & value) const;

		Memento & operator<<(const size_t & value);
		const Memento& operator>>(size_t & value) const;

		Memento & operator<<(const float & value);
		const Memento& operator>>(float & value) const;

		Memento & operator<<(const double & value);
		const Memento& operator>>(double & value) const;

		Memento & operator<<(const std::string & s);
		const Memento& operator>>(std::string & s) const;

		Memento & operator<<(const Vector2f & v);
		const Memento& operator>>(Vector2f & v) const;

		Memento & operator<<(const Color4f & c);
		const Memento& operator>>(Color4f & c) const;

		/**
		 * Stores raw bytes in this memento
		 *
		 * @param bytes	pointer to the byte array
		 * @param the number of bytes to write
		 */
		Memento & Write(const unsigned char * bytes, size_t count);

		/**
		 * Reads raw bytes from this memento.
		 *
		 * @param bytes	pointer to the byte array
		 * @param the number of bytes to read
		 * @return reference to this memento for method chaining
		 */
		const Memento & Read(unsigned char * bytes, size_t count) const;

		/**
		 * Rewinds this memento.
		 * The reading position is set to zero.		 
		 */
		const Memento & Rewind() const;

		/**
		 * Releases all data of this memento.
		 *
		 * @return reference to this memento for method chaining
		 */
		Memento & Reset();

		/**
		 * Returns the raw data stored in this memento.
		 *
		 * @return this memento's data
		 */
		const std::vector<unsigned char> GetData() const {
			return data;
		}

	private:
		/** The data of this memento. */
		std::vector<unsigned char> data;

		/** The current reading position. */
		mutable size_t pos;
	};

} // end of namespace