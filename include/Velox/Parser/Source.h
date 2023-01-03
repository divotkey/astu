/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "TokenType.h"
#include "Script/Scanner.h"

// C++ Standard Library includes
#include <istream>
#include <memory>

namespace velox {

    /**
     * Represents a piece of source code and can scan and tokenize it.
     */
    class Source {
    public:

        /**
         * Constructor.
         */
        Source();

        /**
         * Virtual destructor.
         */
        virtual ~Source() {}

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
         * Returns the current token as retrieved from `GetNextTokenType`.
         *
         * @return the current token
         */
        TokenType GetCurrentTokenType() const;

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

        /**
         * Returns a string representation of the current token.
         *
         * @return string representation of the curren token
         */
        std::string GetTokenTypeAsString() const;

    protected:

        /**
         * Resets this source to its initial state.
         */
        void Reset();

        /** Returns the stream from which to read this source code. */
        virtual std::shared_ptr<std::istream> GetStream() = 0;


    private:
        /** The scanner used to scan for tokens. */
        std::unique_ptr<astu::Scanner> scanner;

        /** Indicates whether scanning for token has already been started. */
        bool scanningStarted;

        /** Creates the scanner used to scan for tokens. */
        static std::unique_ptr<astu::Scanner> BuildScanner();

        /** Internal state. */
        class State {
        public:

            virtual ~State() {}

            virtual TokenType PeekNextToken(Source &source) = 0;
            virtual TokenType GetNextToken(Source &source) = 0;
            virtual TokenType GetCurrentToken(const Source &source) const = 0;
            virtual const std::string& GetStringValue(const Source& source) const = 0;
            virtual int GetIntegerValue(const Source& source) const = 0;
            virtual double GetRealValue(const Source& source) const = 0;
            virtual unsigned int GetLineNumber(const Source& source) const = 0;
        };

        class FirstToken : public State {
        public:
            TokenType PeekNextToken(Source &source) override;
            TokenType GetNextToken(Source &source) override;
            TokenType GetCurrentToken(const Source &source) const override;
            const std::string& GetStringValue(const Source& source) const override;
            int GetIntegerValue(const Source& source) const override;
            double GetRealValue(const Source& source) const override;
            unsigned int GetLineNumber(const Source& source) const override;
        };

        class NotPeeked : public State {
        public:
            TokenType PeekNextToken(Source &source) override;
            TokenType GetNextToken(Source &source) override;
            TokenType GetCurrentToken(const Source &source) const override;
            const std::string& GetStringValue(const Source& source) const override;
            int GetIntegerValue(const Source& source) const override;
            double GetRealValue(const Source& source) const override;
            unsigned int GetLineNumber(const Source& source) const override;
        };

        class Peeked : public State {
        public:
            TokenType PeekNextToken(Source &source) override;
            TokenType GetNextToken(Source &source) override;
            TokenType GetCurrentToken(const Source &source) const override;
            const std::string& GetStringValue(const Source& source) const override;
            int GetIntegerValue(const Source& source) const override;
            double GetRealValue(const Source& source) const override;
            unsigned int GetLineNumber(const Source& source) const override;

            void Store(astu::Scanner& scanner);

        private:
            TokenType token;
            std::string sValue;
            int iValue;
            double rValue;
            unsigned int lineNumber;
        };

        FirstToken firstToken;
        NotPeeked notPeeked;
        Peeked peeked;
        State *curState;
    };

} // end of namespace
