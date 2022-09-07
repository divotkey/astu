// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "Velox/Parser/Token.h"

const std::string velox::Token::GetTypeAsString() const {
    switch (type) {
        case TokenType::INVALID:
            return "INVALID";
        case TokenType::LEFT_BRACKET:
            return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET:
            return "LEFT_BRACKET";
        case TokenType::LEFT_PARENTHESIS:
            return "LEFT_PARENTHESIS";
        case TokenType::RIGHT_PARENTHESIS:
            return "RIGHT_PARENTHESIS";
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
        case TokenType::IDENT:
            return "IDENT";
        case TokenType::TRUE:
            return "TRUE";
        case TokenType::FALSE:
            return "FALSE";
        case TokenType::EOS:
            return "EOS";
        default:
            return "UNDEFINED";
    }
}

bool velox::Token::IsValid() const {
    return type != TokenType::INVALID && type != TokenType::EOS;
}