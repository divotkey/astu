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

    TokenType Source::GetCurrentTokenType() const {
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

    unsigned int Source::GetLineNumber() const {
        return scanner->GetTokenLine();
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
        builder.AddKeyword("for", TOK2INT(TokenType::FOR));
        builder.AddKeyword("class", TOK2INT(TokenType::CLASS));
        builder.AddKeyword("new", TOK2INT(TokenType::NEW));
        builder.AddKeyword("global", TOK2INT(TokenType::GLOBAL));
        builder.AddKeyword("import", TOK2INT(TokenType::IMPORT));

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
            case TokenType::BLOCK_START:
                return "BLOCK_START";
            case TokenType::BLOCK_END:
                return "BLOCK_END";
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
            case TokenType::INCREMENT:
                return "INCREMENT";
            case TokenType::DECREMENT:
                return "DECREMENT";
            case TokenType::EQUAL:
                return "EQUAL";
            case TokenType::NOT_EQUAL:
                return "NOT_EQUAL";
            case TokenType::LESS_THAN:
                return "LESS_THAN";
            case TokenType::LESS_EQUAL:
                return "LESS_EQUAL";
            case TokenType::GREATER_THAN:
                return "GREATER_THAN";
            case TokenType::GREATER_EQUAL:
                return "GREATER_THAN";
            case TokenType::NOT:
                return "NOT";
            case TokenType::LOG_AND:
                return "LOG_AND";
            case TokenType::LOG_OR:
                return "LOG_OR";
            case TokenType::BIN_AND:
                return "BIN_AND";
            case TokenType::BIN_OR:
                return "BIN_OR";
            case TokenType::ASSIGNMENT:
                return "ASSIGNMENT";
            case TokenType::ASSIGN_ADD:
                return "ASSIGN_ADD";
            case TokenType::ASSIGN_SUB:
                return "ASSIGN_SUB";
            case TokenType::ASSIGN_MUL:
                return "ASSIGN_MUL";
            case TokenType::ASSIGN_DIV:
                return "ASSIGN_DIV";
            case TokenType::ASSIGN_MOD:
                return "ASSIGN_MOD";
            case TokenType::FALSE:
                return "FALSE";
            case TokenType::IF:
                return "IF";
            case TokenType::ELSE:
                return "ELSE";
            case TokenType::WHILE:
                return "WHILE";
            case TokenType::DO:
                return "DO";
            case TokenType::LOOP:
                return "LOOP";
            case TokenType::EXIT:
                return "EXIT";
            case TokenType::FOR:
                return "FOR";
            case TokenType::CLASS:
                return "CLASS";
            case TokenType::NEW:
                return "NEW";
            case TokenType::GLOBAL:
                return "GLOBAL";
            case TokenType::IMPORT:
                return "IMPORT";
            case TokenType::FUNCTION:
                return "FUNCTION";
            case TokenType::RETURN:
                return "RETURN";
            case TokenType::IDENT:
                return "IDENT";
            case TokenType::EOS:
                return "EOS";
        }
        return std::string();
    }

    std::string Source::GetTokenTypeAsString() const {
        return TokenTypeToString(GetCurrentTokenType());
    }

}
