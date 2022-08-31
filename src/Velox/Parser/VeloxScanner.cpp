// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "VeloxScanner.h"
#include "Velox/Parser/TokenType.h"
#include "Script/ScannerBuilder.h"

#define IDENT_START_SYMBOLS "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define IDENT_SYMBOLS       "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"
#define WHITE_SPACE_SYMBOLS " \t\r\n\t"
#define STRING_SYMBOLS      " !#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
#define TOK2INT(a) static_cast<int>(a)

using namespace astu;

namespace velox {

    VeloxScanner::VeloxScanner() {
        ScannerBuilder builder;

        builder.SetEndOfSourceToken(TOK2INT(TokenType::EOS));
        builder.AddWhitespaceSymbols(WHITE_SPACE_SYMBOLS);
        builder.SetBlockComment("/*", "*/");
        builder.SetLineComment("//");

        builder.SetStringToken(TOK2INT(TokenType::STRING));
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
        builder.AddKeyword("(", TOK2INT(TokenType::RIGHT_PARENTHESIS));

        builder.AddKeyword("=", TOK2INT(TokenType::ASSIGNMENT));
        builder.AddKeyword("+", TOK2INT(TokenType::ADD));
        builder.AddKeyword("-", TOK2INT(TokenType::SUB));
        builder.AddKeyword("*", TOK2INT(TokenType::MUL));
        builder.AddKeyword("/", TOK2INT(TokenType::DIV));
        builder.AddKeyword("%", TOK2INT(TokenType::MOD));

        scanner = builder.Build();
    }

}