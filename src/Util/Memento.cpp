/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Util/Memento.h"

// C++ Standard Library includes
#include <algorithm>

namespace astu {

	Memento::Memento()
		: pos(0)
	{
		// Intentionally left empty
	}

	Memento::Memento(const std::vector<unsigned char>& data)
		: data(data)
	{
		// Intentionally left empty
	}

	Memento & Memento::operator<<(const std::vector<unsigned char> & data)
	{
		return Write(data.data(), data.size());
	}

	Memento & Memento::operator<<(const int64_t & value)
	{
		return Write(reinterpret_cast<const unsigned char*>(&value), sizeof(value));
	}

	const Memento & Memento::operator>>(int64_t & value) const
	{
		return Read(reinterpret_cast<unsigned char*>(&value), sizeof(value));
	}

	Memento & Memento::operator<<(const uint64_t & value)
	{
		return Write(reinterpret_cast<const unsigned char*>(&value), sizeof(value));
	}

	const Memento & Memento::operator>>(uint64_t & value) const
	{
		return Read(reinterpret_cast<unsigned char*>(&value), sizeof(value));
	}

	Memento & Memento::operator<<(const int & value)
	{
		return Write(reinterpret_cast<const unsigned char*>(&value), sizeof(value));
	}

	const Memento & Memento::operator>>(int & value) const
	{
		return Read(reinterpret_cast<unsigned char*>(&value), sizeof(value));
	}

	Memento & Memento::operator<<(const unsigned int & value)
	{
		return Write(reinterpret_cast<const unsigned char*>(&value), sizeof(value));
	}

	const Memento & Memento::operator>>(unsigned int & value) const
	{
		return Read(reinterpret_cast<unsigned char*>(&value), sizeof(value));
	}

	Memento & Memento::operator<<(const float & value)
	{
		return Write(reinterpret_cast<const unsigned char*>(&value), sizeof(value));
	}

	const Memento & Memento::operator>>(float & value) const
	{
		return Read(reinterpret_cast<unsigned char*>(&value), sizeof(value));
	}

	Memento & Memento::operator<<(const double & value)
	{
		return Write(reinterpret_cast<const unsigned char*>(&value), sizeof(value));
	}

	const Memento & Memento::operator>>(double & value) const
	{
		return Read(reinterpret_cast<unsigned char*>(&value), sizeof(value));
	}

	Memento & Memento::operator<<(const std::string & s)
	{
		return Write(reinterpret_cast<const unsigned char*>(s.c_str()), s.size() + 1);
	}

	const Memento & Memento::operator>>(std::string & s) const
	{
		const unsigned char * start = data.data() + pos;
		const unsigned char * end = data.data() + data.size();
		const unsigned char * ptr = start;
		while (ptr < end && *ptr) {
			++ptr;
		}
		if (ptr >= end) {
			throw std::logic_error("Unable to read string, buffer underflow (memento is out of data)");
		}

		size_t lng = ptr - start;
		s.append(reinterpret_cast<const char*>(start), lng);
		pos += lng + 1;
		return *this;
	}

	Memento & Memento::operator<<(const Vector2f & v)
	{
		*this << v.x << v.y;
		return *this;
	}

	const Memento & Memento::operator>>(Vector2f & v) const
	{
		*this >> v.x >> v.y;
		return *this;
	}

	Memento & Memento::operator<<(const Color4f & c)
	{
		*this << c.r << c.g << c.b << c.a;
		return *this;
	}

	const Memento & Memento::operator>>(Color4f & c) const
	{
		*this >> c.r >> c.g >> c.b >> c.a;
		return *this;
	}

	Memento & Memento::Write(const unsigned char * bytes, size_t count)
	{
		data.insert(data.end(), bytes, bytes + count);
		return *this;
	}

	const Memento & Memento::Read(unsigned char * bytes, size_t count) const
	{
		if (pos + count > data.size()) {
			throw std::logic_error("Buffer underflow (memento is out of data)");
		}

		std::copy(
			data.data() + pos,
			data.data() + pos + count,
			bytes
		);
		pos += count;
		return *this;
	}

	const Memento & Memento::Rewind() const
	{
		pos = 0;
		return *this;
	}

	Memento & Memento::Reset()
	{
		data.clear();
		pos = 0;
		return *this;
	}

} // end of namespace