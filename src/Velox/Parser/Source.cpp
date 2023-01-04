/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Parser/Source.h"
#include "Script/ScannerBuilder.h"

// C++ Standard Library includes
#include <cassert>
#include <iostream>
#include <cctype>

using namespace astu;
using namespace std;

#define IDENT_START_SYMBOLS "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define IDENT_SYMBOLS       "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"
#define WHITE_SPACE_SYMBOLS " \t\r\n\t"
#define STRING_SYMBOLS      " !#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
#define TOK2INT(a) static_cast<int>(a)


namespace velox {

    Source::Source() : scanner(BuildScanner()), scanningStarted(false), curState(&firstToken)
    {
        // Intentionally left empty.
    }

    void Source::Reset()
    {
        curState = &firstToken;
        scanningStarted = false;
        GetNextTokenType();
    }

    TokenType Source::GetNextTokenType() {
        return curState->GetNextToken(*this);
    }

    TokenType Source::GetCurrentTokenType() const {
        return curState->GetCurrentToken(*this);
    }

    TokenType Source::PeekNextTokenType() {
        return curState->PeekNextToken(*this);
    }

    const std::string &Source::GetStringValue() const {
        return curState->GetStringValue(*this);
    }

    int Source::GetIntegerValue() const {
        return curState->GetIntegerValue(*this);
    }

    double Source::GetRealValue() const {
        return curState->GetRealValue(*this);
    }

    unsigned int Source::GetLineNumber() const {
        return curState->GetLineNumber(*this);
    }

    std::unique_ptr<astu::Scanner> Source::BuildScanner() {
        ScannerBuilder builder;

        builder.SetIllegalToken(TOK2INT(TokenType::INVALID));
        builder.SetEndOfSourceToken(TOK2INT(TokenType::EOS));
        builder.AddWhitespaceSymbols(WHITE_SPACE_SYMBOLS);
        builder.SetBlockComment("/*", "*/");
        builder.SetLineComment("//");

        builder.SetIdent(IDENT_START_SYMBOLS, IDENT_SYMBOLS, TOK2INT(TokenType::IDENT));

        builder.SetStringToken(TOK2INT(TokenType::STRING));
        builder.SetStringDelimiter('"');
        builder.SetStringSymbols(STRING_SYMBOLS);
        builder.SetEscapeCharacter('\\');
        builder.AddEscapeSequence('\\', '\\');
        builder.AddEscapeSequence('n', '\n');
        builder.AddEscapeSequence('r', '\r');
        builder.AddEscapeSequence('t', '\t');
        builder.AddEscapeSequence('b', '\b');
        builder.AddEscapeSequence('"', '"');

        builder.SetIntegerToken(TOK2INT(TokenType::INTEGER));
        builder.SetRealToken(TOK2INT(TokenType::REAL));

        builder.AddKeyword("(", TOK2INT(TokenType::LEFT_PARENTHESIS));
        builder.AddKeyword(")", TOK2INT(TokenType::RIGHT_PARENTHESIS));
        builder.AddKeyword("{", TOK2INT(TokenType::BLOCK_START));
        builder.AddKeyword("}", TOK2INT(TokenType::BLOCK_END));
        builder.AddKeyword("[", TOK2INT(TokenType::LEFT_BRACKET));
        builder.AddKeyword("]", TOK2INT(TokenType::RIGHT_BRACKET));
        builder.AddKeyword(";", TOK2INT(TokenType::SEMICOLON));
        builder.AddKeyword(".", TOK2INT(TokenType::MEMBER_ACCESS));
        builder.AddKeyword(",", TOK2INT(TokenType::COMMA));

        builder.AddKeyword("=", TOK2INT(TokenType::ASSIGNMENT));
        builder.AddKeyword("+=", TOK2INT(TokenType::ASSIGN_ADD));
        builder.AddKeyword("-=", TOK2INT(TokenType::ASSIGN_SUB));
        builder.AddKeyword("*=", TOK2INT(TokenType::ASSIGN_MUL));
        builder.AddKeyword("/=", TOK2INT(TokenType::ASSIGN_DIV));
        builder.AddKeyword("%=", TOK2INT(TokenType::ASSIGN_MOD));
        builder.AddKeyword("++", TOK2INT(TokenType::INCREMENT));
        builder.AddKeyword("--", TOK2INT(TokenType::DECREMENT));

        builder.AddKeyword("+", TOK2INT(TokenType::ADD));
        builder.AddKeyword("-", TOK2INT(TokenType::SUB));
        builder.AddKeyword("*", TOK2INT(TokenType::MUL));
        builder.AddKeyword("/", TOK2INT(TokenType::DIV));
        builder.AddKeyword("%", TOK2INT(TokenType::MOD));
        builder.AddKeyword("==", TOK2INT(TokenType::EQUAL));
        builder.AddKeyword("!=", TOK2INT(TokenType::NOT_EQUAL));
        builder.AddKeyword("<", TOK2INT(TokenType::LESS_THAN));
        builder.AddKeyword("<=", TOK2INT(TokenType::LESS_EQUAL));
        builder.AddKeyword(">", TOK2INT(TokenType::GREATER_THAN));
        builder.AddKeyword(">=", TOK2INT(TokenType::GREATER_EQUAL));
        builder.AddKeyword("!", TOK2INT(TokenType::NOT));
        builder.AddKeyword("&&", TOK2INT(TokenType::LOG_AND));
        builder.AddKeyword("||", TOK2INT(TokenType::LOG_OR));
        builder.AddKeyword("&", TOK2INT(TokenType::BIN_AND));
        builder.AddKeyword("|", TOK2INT(TokenType::BIN_OR));
        builder.AddKeyword("function", TOK2INT(TokenType::FUNCTION));
        builder.AddKeyword("return", TOK2INT(TokenType::RETURN));
        builder.AddKeyword("if", TOK2INT(TokenType::IF));
        builder.AddKeyword("else", TOK2INT(TokenType::ELSE));
        builder.AddKeyword("while", TOK2INT(TokenType::WHILE));
        builder.AddKeyword("do", TOK2INT(TokenType::DO));
        builder.AddKeyword("loop", TOK2INT(TokenType::LOOP));
        builder.AddKeyword("exit", TOK2INT(TokenType::EXIT));
        builder.AddKeyword("break", TOK2INT(TokenType::BREAK));
        builder.AddKeyword("continue", TOK2INT(TokenType::CONTINUE));
        builder.AddKeyword("for", TOK2INT(TokenType::FOR));
        builder.AddKeyword("class", TOK2INT(TokenType::CLASS));
        builder.AddKeyword("instant", TOK2INT(TokenType::INSTANT));
        builder.AddKeyword("new", TOK2INT(TokenType::NEW));
        builder.AddKeyword("global", TOK2INT(TokenType::GLOBAL));
        builder.AddKeyword("import", TOK2INT(TokenType::IMPORT));
        builder.AddKeyword("true", TOK2INT(TokenType::TRUE));
        builder.AddKeyword("false", TOK2INT(TokenType::FALSE));
        builder.AddKeyword("undefined", TOK2INT(TokenType::UNDEFINED));

        return builder.Build();
    }

    std::string Source::GetTokenTypeAsString() const {
        return TokenTypeUtils::TokenTypeToString(GetCurrentTokenType());
    }

    /////////////////////////////////////////////////
    /////// Internal state Source::FirstToken
    /////////////////////////////////////////////////

    TokenType Source::FirstToken::GetNextToken(Source &source) {
        source.scanner->Reset(source.GetStream());
        source.curState = &source.notPeeked;
        source.scanner->GetNextToken();
        return static_cast<TokenType>(source.scanner->GetToken());
    }

    TokenType Source::FirstToken::PeekNextToken(Source &source) {
        throw std::logic_error("Illegal state, unable to peek next token");
    }

    TokenType Source::FirstToken::GetCurrentToken(const Source &source) const {
        return static_cast<TokenType>(source.scanner->GetToken());
    }

    const std::string &Source::FirstToken::GetStringValue(const Source &source) const {
        throw std::logic_error("Illegal state, unable to retrieve string from source");
    }

    int Source::FirstToken::GetIntegerValue(const Source &source) const {
        throw std::logic_error("Illegal state, unable to retrieve integer value from source");
    }

    double Source::FirstToken::GetRealValue(const Source &source) const {
        throw std::logic_error("Illegal state, unable to retrieve real value from source");
    }

    unsigned int Source::FirstToken::GetLineNumber(const Source& source) const {
        throw std::logic_error("Illegal state, unable to retrieve line number of token from source");
    }

    /////////////////////////////////////////////////
    /////// Internal state Source::NotPeeked
    /////////////////////////////////////////////////

    TokenType Source::NotPeeked::PeekNextToken(Source &source) {
        source.peeked.Store(*source.scanner);
        source.curState = &source.peeked;
        source.scanner->GetNextToken();
        return static_cast<TokenType>(source.scanner->GetToken());
    }

    TokenType Source::NotPeeked::GetNextToken(Source &source) {
        source.scanner->GetNextToken();
        return static_cast<TokenType>(source.scanner->GetToken());
    }

    TokenType Source::NotPeeked::GetCurrentToken(const Source &source) const {
        return static_cast<TokenType>(source.scanner->GetToken());
    }

    const std::string &Source::NotPeeked::GetStringValue(const Source &source) const {
        return source.scanner->GetStringValue();
    }

    int Source::NotPeeked::GetIntegerValue(const Source &source) const {
        return source.scanner->GetIntegerValue();
    }

    double Source::NotPeeked::GetRealValue(const Source &source) const {
        return source.scanner->GetRealValue();
    }

    unsigned int Source::NotPeeked::GetLineNumber(const Source& source) const {
        return static_cast<unsigned int>(source.scanner->GetTokenLine());
    }

    /////////////////////////////////////////////////
    /////// Internal state Source::Peeked
    /////////////////////////////////////////////////

    TokenType Source::Peeked::GetNextToken(Source &source) {
        auto result = static_cast<TokenType>(source.scanner->GetToken());
        source.curState = &source.notPeeked;
        return result;
    }

    TokenType Source::Peeked::GetCurrentToken(const Source &source) const {
        return token;
    }

    const std::string &Source::Peeked::GetStringValue(const Source &source) const {
        return sValue;
    }

    int Source::Peeked::GetIntegerValue(const Source &source) const {
        return iValue;
    }

    double Source::Peeked::GetRealValue(const Source &source) const {
        return rValue;
    }

    unsigned int Source::Peeked::GetLineNumber(const Source& source) const {
        return lineNumber;
    }

    void Source::Peeked::Store(Scanner &scanner) {
        token = static_cast<TokenType>(scanner.GetToken());
        lineNumber = static_cast<unsigned int>(scanner.GetTokenLine());

        switch (token) {
            case TokenType::INTEGER:
                iValue = scanner.GetIntegerValue();
                break;

            case TokenType::REAL:
                rValue = scanner.GetRealValue();
                break;

            case TokenType::IDENT:
            case TokenType::STRING:
                sValue = scanner.GetStringValue();
                break;
        }
    }

    TokenType Source::Peeked::PeekNextToken(Source &source) {
        throw std::logic_error("Illegal state, unable to peek next token from source");
    }

} // end of namespace
