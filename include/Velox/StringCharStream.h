/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/ICharStream.h"

// C++ Standard Library includes
#include <string>

namespace astu {

	class StringCharStream : public ICharStream {
	public:

		/**
		 * Constructor.
		 * 
		 * @param s	the initial string
		 */
		StringCharStream(const std::string & s = "");

		void reset(const std::string & s = "");

		// Inherited via ICharStream
		virtual bool isEos() const override;
		virtual char nextChar() override;

	private:
		std::string m_str;
		std::string::const_iterator m_pos;
	};

} // end of namespace