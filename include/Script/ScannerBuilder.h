// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Scanner.h"
#include "FSM/NStateMachine.h"
#include "FSM/PowersetBuilder.h"

// C++ Standard Library
#include <optional>
#include <set>
#include <map>

namespace astu {

    class ScannerBuilder {
    public:

        /**
         * Constructor.
         */
        ScannerBuilder();

        ScannerBuilder &AddIdentSymbol(char ch);
        ScannerBuilder &AddIdentStartSymbol(char ch);
        ScannerBuilder &SetIdentToken(int token);
        ScannerBuilder &SetIdent(const std::string &startSymbols, const std::string &symbols, int token);

        ScannerBuilder &AddWhitespaceSymbol(char ch);
        ScannerBuilder &AddWhitespaceSymbols(const std::string &whitespaces);

        ScannerBuilder &SetEndOfSourceToken(int token);
        ScannerBuilder &SetIllegalToken(int token);
        ScannerBuilder &SetIntegerToken(int token);
        ScannerBuilder &SetRealToken(int token);
        ScannerBuilder &SetDecimalSeparator(char ch);
        ScannerBuilder &SetStringDelimiter(char ch);
        ScannerBuilder &SetEscapeCharacter(char ch);
        ScannerBuilder &AddEscapeSequence(char sequenceChar, char targetChar);

        ScannerBuilder &SetBlockComment(const std::string &commentStart, const std::string &commentEnd);
        ScannerBuilder &SetLineComment(const std::string &commentStart);

        ScannerBuilder &SetStringToken(int token);
        ScannerBuilder &AddStringSymbol(char ch);
        ScannerBuilder &SetStringSymbols(const std::string &symbols);

        ScannerBuilder &AddKeyword(const std::string& keyword, int token);
        bool HasKeyword(const std::string& keyword) const;

        ScannerBuilder &Reset();
        std::unique_ptr<Scanner> Build();

    private:
        /** Used to convert non-deterministic state machine into deterministic state machine. */
        astu::PowersetBuilder powersetBuilder;

        /** The non-deterministic state machine used to build the deterministic state machine. */
        astu::NStateMachine nsm;

        /** All allowed start symbols for identifiers. */
        std::set<char> identStartSymbols;

        /** All allowed symbols for identifiers, exclusive start symbols. */
        std::set<char> identSymbols;

        /** All whitespace symbols. */
        std::set<char> whitespaceSymbols;

        /** All whitespace symbols. */
        std::set<char> stringSymbols;

        /** Add accepted symbols. */
        std::set<char> symbols;

        /** Contains required special tokens, e.g., ident, integer, end-of-source, etc. */
        std::map<int, int> specialTokens;

        /** The keywords and their associated tokens. */
        std::map<const std::string, int> keywords;

        /** Symbol used to separate the integer part from the fractional part of a number.*/
        char decimalSeparator;

        /** Symbol used to delimit string literals. */
        char stringDelimiter;

        /** The escape character used to include special characters within strings. */
        char escapeCharacter;

        /** Symbol sequence that introduces a block comment. */
        std::string blockCommentStart;

        /** Symbol sequence that ends a block comment. */
        std::string blockCommentEnd;

        /** Symbol sequence that starts a line comment. */
        std::string lineComment;

        /** Maps escape sequences to its corresponding characters. */
        std::map<char, char> escapeSequences;

        /** Maps error message IDs to indexes used by the scanner. */
        std::map<int, size_t> errorMessageIndexes;

        /** The scanner that is currently built. */
        std::unique_ptr<Scanner> scanner;

        void BuildSymbols();
        void BuildErrorMessages();
        void BuildStartState();
        void BuildWhitespaceState();
        void BuildIdentState();
        void BuildEndOfSourceState();
        void BuildNonIdentKeywordState(const std::string& keyword, int token);
        void BuildIdentKeywordState(const std::string& keyword, int token);
        void BuildNumberState();
        void BuildStringState();
        void BuildEscapeStates();
        void BuildBlockCommentStates();
        void BuildLineCommentStates();
        void BuildInvalidCharState();
        void VerifyConfiguration() const;
        bool HasSpecialToken(int tokenId) const;
        bool IsKeywordStart(char symbol) const;
        std::function<void (char, void*)> CreateSetTokenFunc(int token, int priority = 0) const;
        std::function<void (char, void*)> CreateSetTokenAndAddCharFunc(int token, int priority = 0) const;
        std::function<void (char, void*)> CreateAddCharFunc(int priority) const;
        std::function<void (char, void*)> CreateErrorFunc(int messageId, int priority) const;

        void BuildBlockCommentSm();
        void BuildLineCommentSm();
    };

}



