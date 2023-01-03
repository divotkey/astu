/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Parser/TokenType.h"
#include "Math/Tuple2.h"

namespace velox {


    class ISource {
    public:

        /**
         * Virtual destructor.
         */
        virtual ~ISource() = default;

        /**
         * Resets this source to its initial state without changing its underlying source.
         */
        virtual void Reset() = 0;

        /**
         * Returns the current token as retrieved from `GetNextTokenType`.
         *
         * @return the current token
         */
        virtual TokenType GetCurrentTokenType() const = 0;

        /**
         * Scans for the next token.
         *
         * @return the detected token
         */
        virtual TokenType GetNextTokenType() = 0;

        /**
         * Returns the string value of an string-token.
         * This method only returns valid results if the current token is of type string.
         *
         * @return the string value of an string-token
         */
        virtual const std::string &GetStringValue() const = 0;

        /**
         * Returns the integer value of an integer-token.
         * This method only returns valid results if the current token is of type integer.
         *
         * @return the integer value of an integer token
         */
        virtual int GetIntegerValue() const = 0;

        /**
         * Returns the floating-point value of an real-token.
         * This method only returns valid results if the current token is of type real.
         *
         * @return the floating-point value of an real-token
         */
        virtual double GetRealValue() const = 0;

        /**
         * Return the row and column number of the current token within the source code.
         *
         * @return the row and column number of the current token
         */
        virtual astu::Tuple2i GetPos() const = 0;

        /**
         * Returns the filepath of the source.
         *
         * @return  the filepath
         */
        virtual std::string GetFilepath() const = 0;

        /**
         * Returns the token which would been detected by the next call to 'GetNextTokenType'.
         *
         * @return the token which will be returned by 'GetNextTokenType'
         */
        virtual TokenType PeekNextTokenType() = 0;

        /**
         * This function checks if the next token will be a block start.
         *
         * This method was introduced for performance reasons, since a store/restore using memento is a relatively
         * expensive operation.
         *
         * @return `true` if the next token will be a block-start token.
         */
        virtual bool IsBlockStartFollowing() const = 0;

        /**
         * Returns the line number of the current token within this source code.
         *
         * @return  the line number of the current token
         */
        int GetLine() const {
            return GetPos().y;
        }

        /**
         * Returns the column number of the current token within this source code.
         *
         * @return  the column number of the current token
         */
        int GetColumn() const {
            return GetPos().x;
        }

    };
} // end of namespace