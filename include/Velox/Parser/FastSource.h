// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "TokenType.h"
#include "Math/Tuple2.h"
#include "Util/Memento.h"

// C++ Standard Library includes
#include <memory>
#include <string>
#include <map>
#include <set>
#include <istream>
#include <fstream>

namespace velox {

    /**
     * Represents a piece of source code offers methods to scan and tokenize it.
     */
    class FastSource {
    public:

        /**
         * Constructor.
         */
        FastSource() = default;

        /**
         * Virtual destructor.
         */
        virtual ~FastSource() {}

        /**
         * Resets this source to its initial state.
         */
        void Reset();

        /**
         * Stores the current state to the specified memento.
         *
         * @param memento   the memento which receives the state
         */
        virtual void Store(astu::Memento &memento);

        /**
         * Restores a previously stored state from the specified memento.
         *
         * @param memento   the memento that contains the state
         */
        virtual void Restore(const astu::Memento &memento);

        /**
         * Returns the current token as retrieved from `GetNextTokenType`.
         *
         * @return the current token
         */
        TokenType GetCurrentTokenType() const {
            return curToken;
        }

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
        const std::string &GetStringValue() const {
            return curString;
        }

        /**
         * Returns the integer value of an integer-token.
         * This method only returns valid results if the current token is of type integer.
         *
         * @return the integer value of an integer token
         */
        int GetIntegerValue() const {
            return curInteger;
        }

        /**
         * Returns the floating-point value of an real-token.
         * This method only returns valid results if the current token is of type real.
         *
         * @return the floating-point value of an real-token
         */
        double GetRealValue() const {
            return curReal;
        }

        /**
         * Returns the line number of the current token within this source code.
         *
         * @return  the line number of the current token
         */
        int GetLine() const;

        /**
         * Returns the column number of the current token within this source code.
         *
         * @return  the column number of the current token
         */
        int GetColumn() const;

        /**
         * Return the row and column number of the current token within the source code.
         *
         * @return the row and column number of the current token
         */
        astu::Tuple2i GetPos() const;

    protected:

        /**
         * Returns the next character within the source.
         *
         * @return the next character or -1 if the end of the source has been reached.
         */
        virtual int NextChar() = 0;

    private:
        /** Associates keywords to token types. */
        static const std::map<std::string, TokenType> keywords;

        /** The current character. */
        char curChar;

        /** The current position within the source. */
        astu::Tuple2i curPos;

        /** The start position of the last recognized token. */
        astu::Tuple2i startPos;

        /** The end position of the last recognized token. */
        astu::Tuple2i endPos;

        /** The last recognized token type. */
        TokenType curToken;

        /** Stores last recognized string value or identifier names. */
        std::string curString;

        /** Stores the last recognized integer value. */
        int curInteger;

        /** Stores the last recognized floating-point value. */
        double curReal;

        /** Used to restore state after peeking the next token. */
        astu::Memento memento;


        /**
         * Skips all whitespace characters.
         *
         * The curChar variable will be either -1 or a non-whitespace character after calling this method.
         */
        void EatWhitespace();

        /**
         * Skips the current line.
         */
        void EatLine();

        /**
         * Skips the block comment.
         * This method assumes, that the start of the block comment has already been consumed.
         */
        void EatBlockComment();

        /**
         * Reads a string which is enclosed within double quote characters.
         */
        void ReadString();

        /**
         * Reads an identifier.
         */
        void ReadIdentifier();

        /**
         * Reads an integer value within double quote characters.
         *
         * @return `true` if the base was 10 and real number sequence could follow
         */
        bool ReadInteger();

        /**
         * Reads a hexadecimal integer value.
         */
        void ReadHexInteger();

        /**
         * Reads a real value.
         */
        void ReadReal();

        /**
         * Reads the exponent value for scientific notation.
         *
         * @return the exponent
         */
        double ReadExponent();

        /**
         * Updates the current character and source position.
         */
        void ReadChar();
    };

    /**
     * This class delivers characters from a file source.
     */
    class FastFileSource : public FastSource {
    public:

        /**
         * Constructor.
         *
         * @param filepath  the file path to the script source
         * @throws std::runtime_error in case the file could not be opened for reading
         */
        FastFileSource(const std::string &filepath);

        /**
         * Resets this source to a new file path.
         *
         * @param filepath the new file path to the source script.
         * @throws std::runtime_error in case the file could not be opened for reading
         */
        void Reset(const std::string &filepath);

        /**
         * Resets this source to the start of the current script source.
         */
        void Reset();

        // Inherited via FastSource;
        void Store(astu::Memento &memento) override;
        void Restore(const astu::Memento &memento) override;

    protected:

        // Inherited via FastSource
        int NextChar() override;

    private:
        /** The path to the input file. */
        std::string filepath;

        /** The file stream to the source file. */
        std::unique_ptr<std::ifstream> source;
    };

} // end of namespace