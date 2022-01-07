
/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <string>

namespace astu {

	/**
	 * Exception thrown in case of an runtime error running a Velox script.
	 */
	class VeloxInterpreterException {
	public:

		/**
		 * Constructor.
		 *
		 * @param message	the error text
		 */
		VeloxInterpreterException(const std::string & message)
			: m_located(false)
		{
			m_message = message;
		}

		/**
		 * Constructor.
		 *
		 * @param message	the error text
		 */
		VeloxInterpreterException(const std::string & message, unsigned int lineNumber)
			: m_located(lineNumber > 0), m_lineNumber(lineNumber)
		{
			m_message = message;
		}

		/**
		 * Returns the error message of this exception including the optional line number.
		 *
		 * @return the error message of this exception
		 */
		std::string getMessage() const {
			if (m_located) {
				return m_message + " at line " + std::to_string(m_lineNumber);
			}
			else {
				return m_message;
			}
		}

		/**
		 * Returns the error message of this exception.
		 *
		 * @return the error message of this exception
		 */
		const std::string & getMessageText() const {
			return m_message;
		}

		bool isLocated() const {
			return m_located;
		}

	private:
		/** The error message of this exception. */
		std::string m_message;

		/** The line number associated with this exception. */
		unsigned int m_lineNumber;

		/* Indicates that a line number/location is associated with this exception. */
		bool m_located;
	};

} // end of namespace
