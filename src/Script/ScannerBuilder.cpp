// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Script/ScannerBuilder.h"

#define START_STATE         "START"
#define STRING_STATE        "STRING"
#define STRING_START_STATE  "STRING_START"
#define STRING_ERROR_STATE  "STRING_ERROR"
#define STRING_END_STATE    "STRING_END"

#define WHITESPACE_TOKEN    -1
#define BLOCK_COMMENT_START -2
#define BLOCK_COMMENT_END   -3
#define LINE_COMMENT_START  -4
#define LINE_COMMENT_END    -5

#define LOW_PRIORITY        -1
#define NORMAL_PRIORITY     0
#define HIGH_PRIORITY       1

// IDs used to access tokens within the token map.
#define IDENT_TOKEN_ID      0
#define EOS_TOKEN_ID        1
#define ILLEGAL_TOKEN_ID    2
#define INTEGER_TOKEN_ID    3
#define INTEGER_TOKEN_ID    3
#define REAL_TOKEN_ID       4
#define STRING_TOKEN_ID     5

// Internal error messages IDs
#define STRING_LITERAL_NOT_CLOSED   1
#define ILLEGAL_ESCAPE_SEQUENCE     2
#define BLOCK_COMMENT_NOT_CLOSED    3
#define INVALID_CHARACTER_DETECTED  4

#include <iostream>
using namespace std;

namespace astu {

    /////////////////////////////////////////////////
    /////// Builder related methods
    /////////////////////////////////////////////////

    ScannerBuilder::ScannerBuilder() {
        Reset();
    }

    ScannerBuilder &ScannerBuilder::AddIdentSymbol(char ch) {
        identSymbols.insert(ch);
        return *this;
    }

    ScannerBuilder &ScannerBuilder::AddIdentStartSymbol(char ch) {
        identStartSymbols.insert(ch);
        return *this;
    }

    ScannerBuilder &ScannerBuilder::SetIdentToken(int token) {
        specialTokens[IDENT_TOKEN_ID] = token;
        return *this;
    }

    ScannerBuilder &ScannerBuilder::SetEndOfSourceToken(int token) {
        specialTokens[EOS_TOKEN_ID] = token;
        return *this;
    }

    ScannerBuilder &ScannerBuilder::SetIllegalToken(int token) {
        specialTokens[ILLEGAL_TOKEN_ID] = token;
        return *this;
    }

    ScannerBuilder &ScannerBuilder::SetIntegerToken(int token) {
        specialTokens[INTEGER_TOKEN_ID] = token;
        return *this;
    }

    ScannerBuilder &ScannerBuilder::SetRealToken(int token) {
        specialTokens[REAL_TOKEN_ID] = token;
        return *this;
    }

    ScannerBuilder &ScannerBuilder::SetStringToken(int token) {
        specialTokens[STRING_TOKEN_ID] = token;
        return *this;
    }

    ScannerBuilder &ScannerBuilder::SetIdent(const string &startSymbols, const string &symbols, int token) {
        SetIdentToken(token);

        identStartSymbols.clear();
        for (auto sym : startSymbols) {
            AddIdentStartSymbol(sym);
        }

        identSymbols.clear();
        for (auto sym : symbols) {
            AddIdentSymbol(sym);
        }

        return *this;
    }

    ScannerBuilder &ScannerBuilder::AddWhitespaceSymbol(char ch) {
        whitespaceSymbols.insert(ch);
        return *this;
    }

    ScannerBuilder &ScannerBuilder::AddWhitespaceSymbols(const string &whitespaces) {
        whitespaceSymbols.clear();
        for (auto sym : whitespaces) {
            AddWhitespaceSymbol(sym);
        }
        return *this;
    }

    bool ScannerBuilder::HasKeyword(const std::string &keyword) const {
        return keywords.find(keyword) != keywords.end();
    }

    ScannerBuilder &ScannerBuilder::AddKeyword(const std::string &keyword, int token) {

        if (keyword.empty()) {
            throw std::logic_error("Keyword must not be empty");
        }

        if (token < 0) {
            throw std::logic_error("Token must be greater or equal zero, got " + to_string(token));
        }

        if (HasKeyword(keyword)) {
            throw std::logic_error("Ambiguous keyword '" + keyword + "'");
        }

        keywords[keyword] = token;
        return *this;
    }

    ScannerBuilder &ScannerBuilder::SetDecimalSeparator(char ch) {
        decimalSeparator = ch;
        return *this;
    }

    ScannerBuilder &ScannerBuilder::SetStringDelimiter(char ch) {
        stringDelimiter = ch;
        return *this;
    }

    ScannerBuilder &ScannerBuilder::SetEscapeCharacter(char ch) {
        escapeCharacter = ch;
        return *this;
    }

    ScannerBuilder &ScannerBuilder::AddEscapeSequence(char sequenceChar, char targetChar) {
        escapeSequences[sequenceChar] = targetChar;
        return *this;
    }

    ScannerBuilder &ScannerBuilder::AddStringSymbol(char ch) {
        stringSymbols.insert(ch);
        return *this;
    }

    ScannerBuilder &ScannerBuilder::SetStringSymbols(const string &symbols) {
        stringSymbols.clear();
        for (auto sym : symbols) {
            AddStringSymbol(sym);
        }
        return *this;
    }

    ScannerBuilder &ScannerBuilder::SetBlockComment(const string &commentStart, const string &commentEnd) {
        blockCommentStart = commentStart;
        blockCommentEnd = commentEnd;
        return *this;
    }

    ScannerBuilder &ScannerBuilder::SetLineComment(const string &commentStart) {
        lineComment = commentStart;
        return *this;
    }

    /////////////////////////////////////////////////
    /////// State machine related methods.
    /////////////////////////////////////////////////

    ScannerBuilder &ScannerBuilder::Reset() {
        assert(symbols.empty());
        identSymbols.clear();
        identStartSymbols.clear();
        specialTokens.clear();
        decimalSeparator = '.';
        stringDelimiter = '\"';
        escapeCharacter = '\\';
        stringSymbols.clear();
        blockCommentStart.clear();
        blockCommentEnd.clear();
        lineComment.clear();
        errorMessageIndexes.clear();
        return *this;
    }

    void ScannerBuilder::BuildSymbols() {
        symbols.insert(whitespaceSymbols.begin(), whitespaceSymbols.end());

        if (HasSpecialToken(IDENT_TOKEN_ID)) {
            symbols.insert(identStartSymbols.begin(), identStartSymbols.end());
            symbols.insert(identSymbols.begin(), identSymbols.end());
        }

        if (HasSpecialToken(STRING_TOKEN_ID)) {
            symbols.insert(stringSymbols.begin(), stringSymbols.end());
            symbols.insert(stringDelimiter);
            symbols.insert(escapeCharacter);
        }

        if (!blockCommentStart.empty() && !blockCommentEnd.empty()) {
            symbols.insert(blockCommentStart.begin(), blockCommentStart.end());
            symbols.insert(blockCommentEnd.begin(), blockCommentEnd.end());
        }

        if (!symbols.empty()) {
            symbols.insert(lineComment.begin(), lineComment.end());
        }

        symbols.insert(decimalSeparator);
        symbols.insert(EOF);

        for (auto it : keywords) {
            symbols.insert(it.first.begin(), it.first.end());
        }

        // FIXME make digits optional based on whether scanner should be able to detect integer and or real numbers.
        for (char ch = '0'; ch <= '9'; ++ch) {
            symbols.insert(ch);
        }
    }

    std::unique_ptr<Scanner> ScannerBuilder::Build() {
        VerifyConfiguration();
        scanner.reset(new Scanner());

        BuildErrorMessages();
        BuildSymbols();
        BuildStartState();
        //BuildInvalidCharState();
        BuildWhitespaceState();
        BuildEndOfSourceState();
        BuildNumberState();
        BuildHexNumberState();

        for (const auto& it : keywords) {
            if (identStartSymbols.find(it.first[0]) == identStartSymbols.end()) {
                BuildNonIdentKeywordState(it.first, it.second);
            } else {
                BuildIdentKeywordState(it.first, it.second);
            }
        }

        if (HasSpecialToken(IDENT_TOKEN_ID)) {
            BuildIdentState();
        }

        if (HasSpecialToken(STRING_TOKEN_ID)) {
            BuildStringState();
            BuildEscapeStates();
        }

        if (!blockCommentStart.empty() && !blockCommentEnd.empty()) {
            BuildBlockCommentStates();
        }

        if (!lineComment.empty()) {
            BuildLineCommentStates();
        }

        scanner->sm = powersetBuilder.Build(nsm);
        scanner->ignoreTokens.insert(WHITESPACE_TOKEN);
        scanner->blockCommentStart = BLOCK_COMMENT_START;
        scanner->lineCommentStart = LINE_COMMENT_START;

        for (size_t i = 0; i < scanner->sm->NumStates(); ++i) {
            if (scanner->sm->IsDeadEndState(i)) {
                // This state must not be an accepting state, because it is also used to detect
                // chars, that do not belong to the previously recognized token. However, this state
                // causes an error condition, if it becomes a real active state.

                scanner->sm->BeginState(i);
                scanner->sm->SetEnterFunc(CreateErrorFunc(INVALID_CHARACTER_DETECTED, NORMAL_PRIORITY));
                scanner->sm->EndState();
            }
        }

        if (!blockCommentStart.empty() && !blockCommentEnd.empty()) {
            BuildBlockCommentSm();
            scanner->blockCommentSm = powersetBuilder.Build(nsm);
        }
        if (!lineComment.empty()) {
            BuildLineCommentSm();
            scanner->lineCommentSm = powersetBuilder.Build(nsm);
        }

        nsm.Clear();
        symbols.clear();

        return std::move(scanner);
    }

    void ScannerBuilder::BuildErrorMessages() {
        assert(errorMessageIndexes.empty());

        errorMessageIndexes[STRING_LITERAL_NOT_CLOSED] = scanner->AddErrorMessage("string literal not closed");
        errorMessageIndexes[ILLEGAL_ESCAPE_SEQUENCE] = scanner->AddErrorMessage("illegal escape sequence");
        errorMessageIndexes[BLOCK_COMMENT_NOT_CLOSED] = scanner->AddErrorMessage("block comment not closed");
        errorMessageIndexes[INVALID_CHARACTER_DETECTED] = scanner->AddErrorMessage("invalid character");
    }

    void ScannerBuilder::BuildStartState() {
        nsm.BeginState();
        nsm.SetStartState(true);
        nsm.SetName(START_STATE);
        nsm.EndState();
    }

    //void ScannerBuilder::BuildInvalidCharState() {
    //    nsm.BeginState();
    //    nsm.SetAccepting(true);
    //    nsm.SetEnterFunc(CreateErrorFunc(INVALID_CHARACTER_DETECTED, NORMAL_PRIORITY));
    //    nsm.SetName(INVALID_CHAR);
    //    nsm.EndState();
    //
    //    nsm.BeginState(nsm.GetStartState());
    //    for (auto ch : symbols) {
    //        if (identStartSymbols.find(ch) != identStartSymbols.end())
    //            continue;
    //
    //        if (!blockCommentStart.empty() && blockCommentStart[0] == ch)
    //            continue;
    //
    //        if (!lineComment.empty() && lineComment[0] == ch)
    //            continue;
    //
    //        if (IsKeywordStart(ch))
    //            continue;
    //    }
    //    nsm.EndState();
    //}

    void ScannerBuilder::BuildWhitespaceState() {
        if (whitespaceSymbols.empty())
            return;

        auto qWhiteSpace = nsm.BeginState();
        nsm.SetAccepting(true);
        nsm.SetEnterFunc(CreateSetTokenFunc(WHITESPACE_TOKEN, NORMAL_PRIORITY));

        for (auto ch : whitespaceSymbols) {
            nsm.AddTransition(ch, qWhiteSpace);
        }
        nsm.EndState();

        nsm.BeginState(nsm.GetState(START_STATE));
        for (auto ch : whitespaceSymbols) {
            nsm.AddTransition(ch, qWhiteSpace);
        }
        nsm.EndState();
    }

    void ScannerBuilder::BuildEndOfSourceState() {
        assert(HasSpecialToken(EOS_TOKEN_ID));

        auto qEndOfSource = nsm.BeginState();
        nsm.SetAccepting(true);
        nsm.SetEnterFunc(CreateSetTokenFunc(specialTokens[EOS_TOKEN_ID], NORMAL_PRIORITY));
        nsm.EndState();

        nsm.BeginState(nsm.GetState(START_STATE));
        nsm.AddTransition(EOF, qEndOfSource);
        nsm.EndState();
    }

    void ScannerBuilder::BuildIdentState() {
        assert(HasSpecialToken(IDENT_TOKEN_ID));

        auto qIdent = nsm.BeginState();
        nsm.SetAccepting(true);
        nsm.SetEnterFunc(CreateSetTokenAndAddCharFunc(specialTokens[IDENT_TOKEN_ID], NORMAL_PRIORITY));

        for (auto ch : identSymbols) {
            nsm.AddTransition(ch, qIdent);
        }
        nsm.EndState();

        auto qIdentStart = nsm.BeginState();
        nsm.SetAccepting(true);
        nsm.AddFlag(Scanner::MARK_TOKEN_START_FLAG);
        nsm.SetEnterFunc(CreateSetTokenAndAddCharFunc(specialTokens[IDENT_TOKEN_ID], NORMAL_PRIORITY));
        for (auto ch : identSymbols) {
            nsm.AddTransition(ch, qIdent);
        }
        nsm.EndState();

        nsm.BeginState(nsm.GetState(START_STATE));
        for (auto ch : identStartSymbols) {
            nsm.AddTransition(ch, qIdentStart);
        }
        nsm.EndState();
    }

    void ScannerBuilder::BuildNonIdentKeywordState(const string &keyword, int token) {
        nsm.BeginState(nsm.GetState(START_STATE));

        bool first = true;
        for (char ch : keyword) {
            auto qNew = nsm.CreateState();
            nsm.AddTransition(ch, qNew);

            nsm.EndState();
            nsm.BeginState(qNew);
            if (first) {
                nsm.AddFlag(Scanner::MARK_TOKEN_START_FLAG);
                first = false;
            }
        }

        nsm.SetAccepting(true);
        nsm.SetEnterFunc(CreateSetTokenFunc(token, NORMAL_PRIORITY));
        nsm.EndState();
    }

    void ScannerBuilder::BuildIdentKeywordState(const string &keyword, int token) {
        nsm.BeginState(nsm.GetState(START_STATE));
        bool first = true;
        for (char ch : keyword) {
            auto qNew = nsm.CreateState();
            nsm.AddTransition(ch, qNew);

            nsm.EndState();
            nsm.BeginState(qNew);
            if (first) {
                nsm.AddFlag(Scanner::MARK_TOKEN_START_FLAG);
                first = false;
            }
            //nsm.AddFlag(Scanner::ADD_STRING_VALUE_CHAR_FLAG);
            nsm.SetEnterFunc(CreateAddCharFunc(NORMAL_PRIORITY));
        }

        nsm.SetAccepting(true);
        nsm.SetEnterFunc(CreateSetTokenAndAddCharFunc(token, HIGH_PRIORITY));
        nsm.EndState();
    }

    void ScannerBuilder::BuildHexNumberState() {
        assert(HasSpecialToken(INTEGER_TOKEN_ID));
        assert(HasSpecialToken(REAL_TOKEN_ID));

        // State for hexadecimal numbers.
        auto qHex = nsm.BeginState();
        nsm.SetAccepting(true);
        int token = specialTokens[INTEGER_TOKEN_ID];
        nsm.SetEnterFunc([token](char ch, void* ctx) {
            //cout << "hex state" << endl;
            if (isdigit(ch)) {
                static_cast<Scanner*>(ctx)->AddIntValue(ch - '0', 16, NORMAL_PRIORITY);
            } else if (ch >= 'a' && ch <= 'f') {
                static_cast<Scanner*>(ctx)->AddIntValue(10 + ch - 'a', 16, NORMAL_PRIORITY);
            } else {
                static_cast<Scanner*>(ctx)->AddIntValue(10 + ch - 'A', 16, NORMAL_PRIORITY);
            }
            static_cast<Scanner*>(ctx)->SetTokenType(token, NORMAL_PRIORITY);
        });

        for (char ch = '0'; ch <= '9'; ++ch) {
            nsm.AddTransition(ch, qHex);
        }

        for (char ch = 'a'; ch <= 'f'; ++ch) {
            nsm.AddTransition(ch, qHex);
        }

        for (char ch = 'A'; ch <= 'F'; ++ch) {
            nsm.AddTransition(ch, qHex);
        }

        // Start state for hexadecimal numbers.
        auto qHexStart2 = nsm.BeginState();
        nsm.SetAccepting(true);
        for (char ch = '0'; ch <= '9'; ++ch) {
            nsm.AddTransition(ch, qHex);
        }

        for (char ch = 'a'; ch <= 'f'; ++ch) {
            nsm.AddTransition(ch, qHex);
        }

        for (char ch = 'A'; ch <= 'F'; ++ch) {
            nsm.AddTransition(ch, qHex);
        }
        //nsm.SetEnterFunc([token](char ch, void* ctx) {
        //    cout << "hex start state 2" << endl;
        //});
        nsm.EndState();

        auto qHexStart1 = nsm.BeginState();
        nsm.AddTransition('x', qHexStart2);
        nsm.AddTransition('X', qHexStart2);
        nsm.SetEnterFunc([token](char ch, void* ctx) {
            //cout << "hex start state 1" << endl;
            static_cast<Scanner*>(ctx)->SetTokenType(token, NORMAL_PRIORITY);
        });
        nsm.EndState();

        nsm.BeginState(nsm.GetState(START_STATE));
        nsm.AddTransition('0', qHexStart1);
        nsm.EndState();
    }

    void ScannerBuilder::BuildNumberState() {
        assert(HasSpecialToken(INTEGER_TOKEN_ID));
        assert(HasSpecialToken(REAL_TOKEN_ID));

        // Assemble states for integer-numbers.
        auto qInteger = nsm.BeginState();
        nsm.SetAccepting(true);

        int token = specialTokens[INTEGER_TOKEN_ID];
        nsm.SetEnterFunc([token](char ch, void* ctx) {
            static_cast<Scanner*>(ctx)->AddIntValue(ch - '0', 10, NORMAL_PRIORITY);
            static_cast<Scanner*>(ctx)->SetTokenType(token, NORMAL_PRIORITY);
        });

        for (char ch = '0'; ch <= '9'; ++ch) {
            nsm.AddTransition(ch, qInteger);
        }
        nsm.EndState();

        // Start state for integer numbers.
        auto qIntegerStart = nsm.BeginState();
        nsm.SetAccepting(true);
        nsm.AddFlag(Scanner::MARK_TOKEN_START_FLAG);
        nsm.SetEnterFunc([token](char ch, void* ctx) {
            static_cast<Scanner*>(ctx)->AddIntValue(ch - '0', 10, NORMAL_PRIORITY);
            static_cast<Scanner*>(ctx)->SetTokenType(token, NORMAL_PRIORITY);
        });

        for (char ch = '0'; ch <= '9'; ++ch) {
            nsm.AddTransition(ch, qInteger);
        }
        nsm.EndState();

        nsm.BeginState(nsm.GetState(START_STATE));
        for (char ch = '0'; ch <= '9'; ++ch) {
            nsm.AddTransition(ch, qIntegerStart);
        }
        nsm.EndState();

        // Assemble states for floating-point numbers.
        auto qRealNumber = nsm.BeginState();
        nsm.SetAccepting(true);
        token = specialTokens[REAL_TOKEN_ID];
        nsm.SetEnterFunc([token](char ch, void* ctx) {
            static_cast<Scanner*>(ctx)->AddRealValue(ch - '0', 10, NORMAL_PRIORITY);
            static_cast<Scanner*>(ctx)->SetTokenType(token, NORMAL_PRIORITY);
        });

        for (char ch = '0'; ch <= '9'; ++ch) {
            nsm.AddTransition(ch, qRealNumber);
        }
        nsm.EndState();

        auto qRealStart = nsm.BeginState();
        nsm.SetAccepting(true);
        nsm.SetEnterFunc([token](char ch, void* ctx) {
            static_cast<Scanner*>(ctx)->SetTokenType(token, LOW_PRIORITY);
        });
        for (char ch = '0'; ch <= '9'; ++ch) {
            nsm.AddTransition(ch, qRealNumber);
        }
        nsm.EndState();

        nsm.BeginState(nsm.GetState(START_STATE));
        nsm.AddTransition(decimalSeparator, qRealStart);
        nsm.EndState();

        nsm.BeginState(qInteger);
        nsm.AddTransition(decimalSeparator, qRealStart);
        nsm.EndState();

        nsm.BeginState(qIntegerStart);
        nsm.AddTransition(decimalSeparator, qRealStart);
        nsm.EndState();
    }

    void ScannerBuilder::BuildEscapeStates() {
        assert(nsm.HasState(STRING_STATE));
        assert(nsm.HasState(STRING_START_STATE));
        assert(nsm.HasState(STRING_ERROR_STATE));
        assert(nsm.HasState(STRING_END_STATE));
        assert(HasSpecialToken(ILLEGAL_TOKEN_ID));

        if (escapeSequences.empty()) {
            if (stringSymbols.find(escapeCharacter) != stringSymbols.end()) {
                nsm.AddTransition(nsm.GetState(STRING_START_STATE), escapeCharacter, nsm.GetState(STRING_STATE));
                nsm.AddTransition(nsm.GetState(STRING_STATE), escapeCharacter, nsm.GetState(STRING_STATE));
            }
            return;
        }

        // Create error state in case of invalid escape sequence.
        auto qEscapeError = nsm.BeginState();
        nsm.SetAccepting(true);
        nsm.SetEnterFunc(CreateErrorFunc(ILLEGAL_ESCAPE_SEQUENCE, NORMAL_PRIORITY));
        nsm.EndState();

        // Create escape sequence start state.
        auto qEscapeStart = nsm.BeginState();
        for (auto ch : symbols) {
            auto it = escapeSequences.find(ch);
            if (it == escapeSequences.end()) {
                nsm.AddTransition(ch, qEscapeError);
            }
        }
        nsm.EndState();

        // Create escape sequence states.
        for (auto it : escapeSequences) {

            auto qEscape = nsm.BeginState();
            char esc = it.second;
            nsm.SetEnterFunc([esc](char, void *ctx){
                static_cast<Scanner *>(ctx)->AddStringValueChar(esc, NORMAL_PRIORITY);
            });

            for (char ch : symbols) {
                if (ch == escapeCharacter) {
                    nsm.AddTransition(escapeCharacter, qEscape);
                } else if (stringSymbols.find(ch) != stringSymbols.end()) {
                    nsm.AddTransition(ch, nsm.GetState(STRING_STATE));
                } else if (ch != stringDelimiter) {
                    nsm.AddTransition(ch, nsm.GetState(STRING_ERROR_STATE));
                }
            }
            nsm.AddTransition(stringDelimiter, nsm.GetState(STRING_END_STATE));
            nsm.EndState();

            nsm.AddTransition(qEscapeStart, it.first, qEscape);
        }

        // Configure string and string start state to accept escape sequences.
        nsm.AddTransition(nsm.GetState(STRING_STATE), escapeCharacter, qEscapeStart);
        nsm.AddTransition(nsm.GetState(STRING_START_STATE), escapeCharacter, qEscapeStart);
    }

    void ScannerBuilder::BuildStringState() {
        assert(HasSpecialToken(STRING_TOKEN_ID));

        // Create string error state, in case string is not closed.
        auto qStringError = nsm.BeginState();
        nsm.SetName(STRING_ERROR_STATE);
        nsm.SetAccepting(true);
        nsm.SetEnterFunc(CreateErrorFunc(STRING_LITERAL_NOT_CLOSED, NORMAL_PRIORITY));
        nsm.EndState();

        // Create string end state.
        auto qStringEnd = nsm.BeginState();
        nsm.SetName(STRING_END_STATE);
        nsm.SetAccepting(true);
        nsm.SetEnterFunc(CreateSetTokenFunc(specialTokens[STRING_TOKEN_ID], NORMAL_PRIORITY));
        nsm.EndState();

        // Create string state.
        auto qString = nsm.BeginState();
        nsm.SetName(STRING_STATE);
        for (auto ch : symbols) {
            nsm.SetEnterFunc(CreateAddCharFunc(NORMAL_PRIORITY));
            if (ch == escapeCharacter) {
                continue;
            } else if (stringSymbols.find(ch) != stringSymbols.end()) {
                nsm.AddTransition(ch, qString);
            } else if (ch != stringDelimiter) {
                nsm.AddTransition(ch, qStringError);
            }
        }
        nsm.AddTransition(stringDelimiter, qStringEnd);
        nsm.EndState();

        // Create string start state.
        auto qStringStart = nsm.BeginState();
        nsm.SetName(STRING_START_STATE);
        nsm.AddFlag(Scanner::MARK_TOKEN_START_FLAG);
        for (auto ch : symbols) {
            if (ch == escapeCharacter) {
                continue;
            } else if (stringSymbols.find(ch) != stringSymbols.end()) {
                nsm.AddTransition(ch, qString);
            } else if (ch != stringDelimiter) {
                nsm.AddTransition(ch, qStringError);
            }
        }
        nsm.EndState();

        // Configure start state to accept strings.
        nsm.BeginState(nsm.GetStartState());
        nsm.AddTransition(stringDelimiter, qStringStart);
        nsm.EndState();
    }

    void ScannerBuilder::BuildBlockCommentStates() {

        // Add (internal) token for block comment start.
        nsm.BeginState(nsm.GetStartState());
        for (auto ch : blockCommentStart) {
            auto q = nsm.CreateState();
            nsm.AddTransition(ch, q);
            nsm.EndState();
            nsm.BeginState(q);
        }

        nsm.SetAccepting(true);
        nsm.SetEnterFunc(CreateSetTokenFunc(BLOCK_COMMENT_START));
        nsm.EndState();
    }

    void ScannerBuilder::BuildLineCommentStates() {

        // Add (internal) token for line comment start.
        nsm.BeginState(nsm.GetStartState());
        for (auto ch : lineComment) {
            auto q = nsm.CreateState();
            nsm.AddTransition(ch, q);
            nsm.EndState();
            nsm.BeginState(q);
        }

        nsm.SetAccepting(true);
        nsm.SetEnterFunc(CreateSetTokenFunc(LINE_COMMENT_START));
        nsm.EndState();
    }

    std::function<void(char, void *)> ScannerBuilder::CreateSetTokenFunc(int token, int priority) const {
        return [token, priority](char, void* ctx) {
            static_cast<Scanner*>(ctx)->SetTokenType(token, priority);
        };
    }

    std::function<void(char, void *)> ScannerBuilder::CreateSetTokenAndAddCharFunc(int token, int priority) const {
        return [token, priority](char ch, void* ctx) {
            static_cast<Scanner*>(ctx)->SetTokenType(token, priority);
            static_cast<Scanner *>(ctx)->AddStringValueChar(ch, priority);
        };
    }

    std::function<void(char, void *)> ScannerBuilder::CreateAddCharFunc(int priority) const {
        return [priority](char ch, void* ctx) {
            static_cast<Scanner *>(ctx)->AddStringValueChar(ch, priority);
        };
    }

    std::function<void(char, void *)> ScannerBuilder::CreateErrorFunc(int messageId, int priority) const {
        size_t messageIndex = errorMessageIndexes.at(messageId);

        return [messageIndex, priority](char ch, void* ctx) {
            //static_cast<Scanner*>(ctx)->SetTokenType(token, priority);
            static_cast<Scanner*>(ctx)->EmitError(messageIndex, priority);
        };
    }

    void ScannerBuilder::VerifyConfiguration() const {
        if (!HasSpecialToken(EOS_TOKEN_ID)) {
            throw std::logic_error("No token specified for end-of-source symbol");
        }

        if (!HasSpecialToken(ILLEGAL_TOKEN_ID)) {
            throw std::logic_error("No token specified for illegal symbols");
        }

        if (!HasSpecialToken(INTEGER_TOKEN_ID)) {
            throw std::logic_error("No token specified for integers");
        }

        if (!HasSpecialToken(REAL_TOKEN_ID)) {
            throw std::logic_error("No token specified for floating-point numbers");
        }

        // Uncomment, if identifier token is not optional.
        //if (!HasSpecialToken(IDENT_TOKEN_ID)) {
        //    throw std::logic_error("No token specified for identifier");
        //}

        if (HasSpecialToken(IDENT_TOKEN_ID)) {
            if (identStartSymbols.empty()) {
                throw std::logic_error("No start symbols defined for identifier");
            }

            if (identSymbols.empty()) {
                throw std::logic_error("No symbols defined for identifier");
            }
        }

        if (stringSymbols.find(stringDelimiter) != stringSymbols.end()) {
            throw std::logic_error("String delimiters must not be part of string symbols");
        }
    }

    bool ScannerBuilder::HasSpecialToken(int tokenId) const {
        return specialTokens.find(tokenId) != specialTokens.end();
    }

    bool ScannerBuilder::IsKeywordStart(char symbol) const {
        for (auto it : keywords) {
            if (it.first[0] == symbol)
                return true;
        }

        return false;
    }

    void ScannerBuilder::BuildBlockCommentSm() {
        nsm.Clear();
        auto qError = nsm.BeginState();
        nsm.SetEnterFunc(CreateErrorFunc(BLOCK_COMMENT_NOT_CLOSED, NORMAL_PRIORITY));
        nsm.SetAccepting(true);
        nsm.EndState();

        auto qStart = nsm.BeginState();
        nsm.SetStartState(true);
        for (auto ch : symbols) {
            if (ch == EOF) {
                nsm.AddTransition(ch, qError);
            } else {
                nsm.AddTransition(ch, qStart);
            }
        }

        for (auto ch : blockCommentEnd) {
            auto q = nsm.CreateState();
            nsm.AddTransition(ch, q);
            nsm.EndState();
            nsm.BeginState(q);
        }

        nsm.SetAccepting(true);
        nsm.SetEnterFunc(CreateSetTokenFunc(BLOCK_COMMENT_END));
        nsm.EndState();
    }

    void ScannerBuilder::BuildLineCommentSm() {
        nsm.Clear();

        auto qEnd = nsm.BeginState();
        nsm.SetAccepting(true);
        nsm.SetEnterFunc(CreateSetTokenFunc(LINE_COMMENT_END));
        nsm.EndState();

        auto qStart = nsm.BeginState();
        nsm.SetStartState(true);
        for (auto ch : symbols) {
            if (ch != EOF && ch != '\n') {
                nsm.AddTransition(ch, qStart);
            }
        }

        nsm.AddTransition(EOF, qEnd);
        nsm.AddTransition('\n', qEnd);
        nsm.EndState();
    }

}
