/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Velox/StringCharStream.h"

// C++ Standard Library includes
#include <cassert>

namespace astu {

	StringCharStream::StringCharStream(const std::string & s)
		: m_str(s), m_pos(m_str.begin())
	{
		// Intentionally left empty
	}

	void StringCharStream::reset(const std::string & s)
	{
		m_str = s;
		m_pos = m_str.begin();
	}

	bool StringCharStream::isEos() const
	{
		return m_pos == m_str.end();
	}
	char StringCharStream::nextChar()
	{
		assert(m_pos != m_str.end());
		return *m_pos++;
	}

} // end of namespace