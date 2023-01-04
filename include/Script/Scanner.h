/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "FSM/FStateMachine.h"

// C++ Standard Library
#include <cassert>
#include <istream>
#include <memory>
#include <queue>
#include <vector>
#include <set>

namespace astu {

    class Scanner {
    public:
        /** Constant representing the end-of-source character. */
        static const char END_OF_SOURCE;

        /** Indicates that a states wants to add the current char to the ident string. */
        static const unsigned int ADD_STRING_VALUE_CHAR_FLAG;

        /** Indicates that a states wants to add the current char to the ident string. */
        static const unsigned int MARK_TOKEN_START_FLAG;


        /** Indicates that a states wants to add the current char as float value. */
        static const unsigned int ADD_FLOAT_VALUE_FLAG;

        /** Deleted copy-constructor. */
        Scanner(const Scanner& other) = delete;

        /** Deleted assign operator. */
        Scanner& operator=(const Scanner &) = delete;

        /**
         * Resets this scanner to start recognizing tokens from he specified source stream.
         *
         * @param inSource the source stream
         */
        void Reset(std::shared_ptr<std::istream> inSource);

        /**
         * Attempts to detect the next token within the source stream.
         */
        void GetNextToken();

        /**
         * Returns the last recognized token.
         *
         * @return the last recognized token
         */
        int GetToken() const {
            return tokenType;
        }

        /**
         * Returns the absolute position of the recognized token, starting at index one.
         *
         * @return
         */
        size_t GetTokenPos() const {
            return tokenPos;
        }

        /**
         * Returns the line number of the recognized token, starting at index one.
         *
         * @return the token's line number
         */
        size_t GetTokenLine() const;

        /**
         * Returns the column number of the recognized token, starting at index one.
         *
         * @return the token's column number
         */
        size_t GetTokenColumn() const;

        /**
         * Returns the string value associated with the last recognized token.
         *
         * @return  the token's string value
         */
        const std::string &GetStringValue() const {
            return stringValue;
        };

        /**
         * Returns the integer value associated with the last recognized token.
         *
         * @return  the token's integer value
         */
        int GetIntegerValue() const {
            return intValue;
        }

        /**
         * Returns the floating-point value associated with the last recognized token.
         *
         * @return  the token's floating-point value
         */
        double GetRealValue() const {
            return intValue + realValue;
        }

        /**
         * Returns the number of states used by the internal state machine.
         *
         * @return the number of internal states
         */
        size_t NumStates() const;

    private:

        struct Command {
            /** The possible command types. */
            enum class Type {SET_TOKEN, ADD_STRING_VALUE_CHAR, ADD_INT_VALUE, ADD_REAL_VALUE, EMIT_ERROR};

            /** The actual type of this command. */
            Type type;

            /** The priority of this command. */
            int priority;

            /** Additional data of this command, holding char, token or integer values etc. */
            int data1;

            /** Additional data of this command, holding multiply factors etc.. */
            int data2;

            /** Constructor. */
            Command(Type type, int priority) : type(type), priority(priority) {};

            static Command CreateSetTokenType(int priority, int token) {
                Command result(Type::SET_TOKEN, priority);
                result.data1 = token;
                return result;
            }

            static Command CreateAddStringValueChar(int priority, char ch) {
                Command result(Type::ADD_STRING_VALUE_CHAR, priority);
                result.data1 = ch;
                return result;
            }

            static Command CreateAddIntValue(int priority, int value, int factor) {
                Command result(Type::ADD_INT_VALUE, priority);
                result.data1 = value;
                result.data2 = factor;
                return result;
            }

            static Command CreateAddRealValue(int priority, int value, int factor) {
                Command result(Type::ADD_REAL_VALUE, priority);
                result.data1 = value;
                result.data2 = factor;
                return result;
            }

            static Command CreateEmitError(int priority, size_t errorIdx) {
                Command result(Type::EMIT_ERROR, priority);
                result.data1 = static_cast<int>(errorIdx);
                return result;
            }

            /**
             * Binary equality operator comparing two commands.
             *
             * @param rhs   the right hand command
             * @return `true` if the specified command's type is equal to this vector
             */
            bool operator==(const Command &rhs) const;

            /**
             * Merges the specified command to this command, according to the priority.
             *
             * @param cmd   the other command
             */
            void Merge(Command &cmd);

            /**
             * Executes this command
             *
             * @param parent    the owning scanner instance
             */
            void Execute(Scanner &parent);
        };

        /** Pending pendingCommands. */
        std::vector<Command> pendingCommands;

        /** Holds a set of commands to be actually executed, used to avoid repetitive memory allocation. */
        std::vector<Command> uniqueCommands;

        /** Used to run through the source code to scan. */
        std::shared_ptr<std::istream> source;

        /** The position of the next character within the source. */
        size_t position;

        /** Stores the positions of newlines, used to calculate line and column from absolute positions. */
        std::vector<size_t> newLines;

        /** The state machine which recognizes the tokens. */
        std::unique_ptr<astu::FStateMachine> sm;

        /** The state machine used to eat block comments. */
        std::unique_ptr<astu::FStateMachine> blockCommentSm;

        /** The state machine used to eat line comments. */
        std::unique_ptr<astu::FStateMachine> lineCommentSm;

        /** A set of token to be read over. */
        std::set<int> ignoreTokens;

        /** The last recognized token type. */
        int tokenType;

        /** The position of the token start of the recognized token. */
        size_t tokenPos;

        /** The string value of an recognized string or ident token. */
        std::string stringValue;

        /** The Integer value of an recognized integer token. */
        int intValue;

        /** Used to assemble floating-point numbers. */
        double realFactor;

        /** The fractional part of a floating-point value. */
        double realValue;

        /** Contains characters that have been put back. */
        std::queue<char> backlog;

        /** List of error messages to be triggered by states. */
        std::vector<std::string> errorMessages;

        /** Token indicating the start of a block comment. */
        int blockCommentStart;

        /** Token indicating the start of a line comment. */
        int lineCommentStart;

        void ScanForNextTokenWithComments();

        void ScanForNextToken(FStateMachine& sm);
        bool IsIgnoreToken(int type) const;
        void PutBack(char ch);
        char GetNextChar();
        void ProcessFlags(unsigned int flags, char ch);

        void SetTokenType(int typeId, int priority);
        void AddStringValueChar(char ch, int priority);
        void AddIntValue(int value, int factor, int priority);
        void AddRealValue(int value, int factor, int priority);
        void EmitError(size_t messageIdx, int priority);
        size_t AddErrorMessage(const std::string& message);

        void ProcessCommands();
        void DiscardCommands();

        /**
         * Private constructor.
         */
        Scanner();

        friend class ScannerBuilder;
    };

}


