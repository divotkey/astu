// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Source.h"
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

    Source::Source() : scanner(BuildScanner()), scanningStarted(false) {
        // Intentionally left empty.
    }

    TokenType Source::GetNextTokenType() {
        if (!scanningStarted) {
            scanner->Reset(GetStream());
            scanningStarted = true;
        }

        scanner->GetNextToken();
        return GetCurrentTokenType();
    }

    TokenType Source::GetCurrentTokenType() {
        return static_cast<TokenType>(scanner->GetToken());
    }

    const std::string &Source::GetStringValue() const {
        return scanner->GetStringValue();
    }

    int Source::GetIntegerValue() const {
        return scanner->GetIntegerValue();
    }

    double Source::GetRealValue() const {
        return scanner->GetRealValue();
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
        builder.AddKeyword(";", TOK2INT(TokenType::SEMICOLON));
        builder.AddKeyword(".", TOK2INT(TokenType::MEMBER_ACCESS));
        builder.AddKeyword(",", TOK2INT(TokenType::COMMA));

        builder.AddKeyword("=", TOK2INT(TokenType::ASSIGNMENT));
        builder.AddKeyword("+", TOK2INT(TokenType::ADD));
        builder.AddKeyword("-", TOK2INT(TokenType::SUB));
        builder.AddKeyword("*", TOK2INT(TokenType::MUL));
        builder.AddKeyword("/", TOK2INT(TokenType::DIV));
        builder.AddKeyword("%", TOK2INT(TokenType::MOD));

        return builder.Build();
    }

    std::string Source::TokenTypeToString(TokenType type) const {
        switch (type) {

            case TokenType::INVALID:
                return "INVALID";
            case TokenType::STRING:
                return "STRING";
            case TokenType::INTEGER:
                return "INTEGER";
            case TokenType::REAL:
                return "REAL";
            case TokenType::LEFT_BRACKET:
                return "LEFT_BRACKET";
            case TokenType::RIGHT_BRACKET:
                return "RIGHT_BRACKET";
            case TokenType::LEFT_PARENTHESIS:
                return "LEFT_PARENTHESIS";
            case TokenType::RIGHT_PARENTHESIS:
                return "RIGHT_PARENTHESIS";
            case TokenType::SEMICOLON:
                return "SEMICOLON";
            case TokenType::COMMA:
                return "COMMA";
            case TokenType::MEMBER_ACCESS:
                return "MEMBER_ACCESS";
            case TokenType::ADD:
                return "ADD";
            case TokenType::SUB:
                return "SUB";
            case TokenType::MOD:
                return "MOD";
            case TokenType::MUL:
                return "MUL";
            case TokenType::DIV:
                return "DIV";
            case TokenType::ASSIGNMENT:
                return "ASSIGNMENT";
            case TokenType::TRUE:
                return "TRUE";
            case TokenType::FALSE:
                return "FALSE";
            case TokenType::IDENT:
                return "IDENT";
            case TokenType::EOS:
                return "EOS";
        }
        return std::string();
    }

}
