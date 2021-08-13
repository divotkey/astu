/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "ICharStream.h"

// C++ Standard Library includes
#include <string>

namespace astu {

	class StringCharStream : public ICharStream {
	public:
		StringCharStream(const std::string & s);

		void reset(const std::string & s);

		// Inherited via ICharStream
		virtual bool isEos() const override;
		virtual char nextChar() override;

	private:
		std::string m_str;
		std::string::const_iterator m_pos;
	};

} // end of namespace