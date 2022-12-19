// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "TokenType.h"

// C++ Standard Library includes
#include <string>
#include <istream>

namespace velox {

    /**
     * Represents a piece of source code offers methods to scan and tokenize it.
     */
    class FastSource {
    public:

        /**
         * Constructor.
         */
        FastSource();

        /**
         * Resets this source to its initial state.
         */
        void Reset(std::string);

        /**
         * Virtual destructor.
         */
        virtual ~FastSource() {}

        /**
         * Returns the current token as retrieved from `GetNextTokenType`.
         *
         * @return the current token
         */
        TokenType GetCurrentTokenType() const;

        /**
         * Scans for the next token.
         *
         * @return the detected token
         */
        TokenType GetNextTokenType();

        /**
         * Returns the token which would been detected by the next call to 'GetNextTokenType'.
         *
         * @return the token which will be returned by 'GetNextTokenType'
         */
        TokenType PeekNextTokenType();

        /**
         * Returns the string value of an string-token.
         * This method only returns valid results if the current token is of type string.
         *
         * @return the string value of an string-token
         */
        const std::string &GetStringValue() const;

        /**
         * Returns the integer value of an integer-token.
         * This method only returns valid results if the current token is of type integer.
         *
         * @return the integer value of an integer token
         */
        int GetIntegerValue() const;

        /**
         * Returns the floating-point value of an real-token.
         * This method only returns valid results if the current token is of type real.
         *
         * @return the floating-point value of an real-token
         */
        double GetRealValue() const;

        /**
         * Returns the line number of the current token within this source code.
         *
         * @return  the line number of the current token
         */
        unsigned int GetLineNumber() const;


    protected:
        /** Returns the stream from which to read this source code. */
        virtual std::istream &GetStream() = 0;

    private:
        /** The current character. */
        char curChar;

        /**
         * Skips all whitespace characters.
         *
         * The curChar variable will be either -1 or a non-whitespace character after calling this method.
         */
        void EatWhitespace();

        void ReadChar();
    };

} // end of namespace