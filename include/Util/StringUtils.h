/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <string>
#include <vector>

namespace astu {

	/**
	 * A utility class providing string related static utility methods.
	 */
	class StringUtils final {
	public:

		/**
		 * Deleted constructor -> no instances of this class allowed.
		 */
		StringUtils() = delete;

		/**
		 * Converts the specified UTF-8 string to UTF-16.
		 *
		 * @param s	the UTF-8 encoded input string
		 * @return the UTF-16 encoded output string
		 */
		static std::wstring utf8ToUtf16(const std::string & s);

		/**
		 * Converts the specified UTF-16 string to UTF-8.
		 *
		 * @param ws	the UTF-16 encoded input string
		 * @return the UTF-8 encoded output string
		 */
		static std::string utf16ToUtf8(const std::wstring & ws);

		/**
		 * Converts the specified UTF-16 string to UTF-8.
		 *
		 * @param wc   the UTF-16 encoded input character
		 * @return the UTF-8 encoded output character
		 */
		static std::string utf16ToUtf8(const wchar_t & wc);

		/**
		 * Removes white space characters on the right side of the string.
		 * This method also removes '\0' null-termination.
		 *
		 * @param s	the string to process
		 * @return reference to the processed string
		 */
		static std::string & rtrim(std::string & s);

		/**
		 * Removes white space characters on the left side of the string.
		 *
		 * @param s	the string to process
		 * @return reference to the processed string
		 */
		static std::string & ltrim(std::string & s);

		/**
		 * Removes white space characters on the left and the right side of the string.
		 *
		 * @param s	the string to process
		 * @return reference to the processed string
		 */
		static std::string & trim(std::string & s);

		/**
		 * Converts the specified amount of bytes to a human readable format
		 *
		 * @return string describing the specified memory size
		 */
		static std::string bytesToString(size_t mem, int precision = 2);

		/**
		 * Converts the specified string to upper case letters in-place.
		 *
		 * @param the string to be converted
		 * @return reference to the specified and now converted string
		 */
		static std::string & toUpperCase(std::string & s);

		/**
		 * Converts the specified string to lower case letters in-place.
		 *
		 * @param the string to be converted
		 * @return reference to the specified and now converted string
		 */
		static std::string & toLowerCase(std::string & s);

		/**
		 * Splits the specified string using the given character as separator.
		 * 
		 * @param s		the string to split
		 * @param ch	the separation character
		 * @return a vector with strings
		 */
		static std::vector<std::string> split(const std::string & s, char ch);

		/**
		 * Converts the specified value to a string in hexadecimal representation.
		 *
		 * @param value	the value to convert
		 * @return the value represented as string in hexadecimal format
		 */
		static std::string toHexString(unsigned long value);

		/**
		 * Converts the specified pointer to a string in hexadecimal representation.
		 *
		 * @param pointer	the pointer to convert
		 * @return the pointer represented as string in hexadecimal format
		 */
		static std::string toString(void *pointer);

		/**
		 * Converts the specified float value to a string using the specified precision.
		 *
		 * @param value	the float value to convert
		 * @param precision	the number of floating point digits
		 * @return the converted string
		 */
		static std::string toString(float value, int precision = 2);

		/**
		 * Converts the specified double value to a string using the specified precision.
		 *
		 * @param value	the double value to convert
		 * @param precision	the number of floating point digits
		 * @return the converted string
		 */
		static std::string toString(double value, int precision = 2);

		/**
		 * Tests if a string starts with given character sequence.
		 *
		 * @param s		the string to test
		 * @param seq	the sequence
		 */
		static bool startsWith(const std::string & s, const std::string & seq);

		/**
		 * Tests if a string ends with given character sequence.
		 *
		 * @param s		the string to test
		 * @param seq	the sequence
		 */
		static bool endsWith(const std::string & s, const std::string & seq);

		/**
		 * Tests if a string starts with given character.
		 *
		 * @param s		the string to test
		 * @param ch	the character
		 */
		static bool startsWith(const std::string & s, char ch);

		/**
		 * Extracts the path from the specified string.
		 *
		 * @param s	a filename including the full path
		 * @return the extracted file path
		 */
		static std::string extractPath(const std::string & s);

		/**
		 * Extracts the filename from the specified string.
		 *
		 * @param s	a filename including the full path
		 * @return the extracted filename
		 */
		static std::string extractFilename(const std::string & s);

		/**
		 * Extracts the filename stripped of its extension.
		 *
		 * @param s	a filename including the full path
		 * @return the stem of the filename
		 */
		static std::string extractStem(const std::string & s);

		/**
		 * Extracts the extension of the filename.
		 *
		 * @param s	a filename including the full path
		 * @return the extension of the filename
		 */
		static std::string extractExtension(const std::string & s);

		/**
		 * Returns the OS specific file path separator.
		 *
		 * @return the file path separator
		 */
		static char pathSeparator();

		/**
		 * Replaces the occurrence of a substring with another string.
		 *
		 * @param s		the string to be processed
		 * @param from	the substring to be replaced
		 * @param to	the string used as replacement
		 * @return		reference to the processed string
		 */
		static std::string & replace(std::string & s, const std::string & from, const std::string & to);

	};

} // end of namespace