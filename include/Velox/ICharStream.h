/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

namespace astu {

	/**
	 * Abstract and simple interface to read characters from a stream.
	 * This interface is used to access text resources in a stream-like manner.
	 */
	class ICharStream {
	public:

		/**
		 * Virtual destructor.
		 */
		virtual ~ICharStream() {}

		/**
		 * Returns whether the end of this stream has been reached.
		 *
		 * @return `true` if the end of stream has been reached
		 */
		virtual bool isEos() const = 0;

		/**
		 * Returns the next character of this stream.
		 * The reading position of the stream will be increased by one character.
		 *
		 * @return the next character
		 * @throws std::logic_error in case the end of this stream has been reached
		 * @throws std::runtime_error in case an I/O error has occurred
		 */
		virtual char nextChar() = 0; 
	};

} // end of namespace