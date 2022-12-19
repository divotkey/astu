// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Velox/Parser/TokenType.h"

using namespace std;

namespace velox {

    std::string TokenTypeUtils::TokenTypeToString(TokenType type) {
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
            case TokenType::TRUE:
                return "TRUE";
            case TokenType::FALSE:
                return "FALSE";
            case TokenType::UNDEFINED:
                return "UNDEFINED";
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
            case TokenType::BREAK:
                return "BREAK";
            case TokenType::CONTINUE:
                return "CONTINUE";
            case TokenType::FOR:
                return "FOR";
            case TokenType::CLASS:
                return "CLASS";
            case TokenType::NEW:
                return "NEW";
            case TokenType::INSTANT:
                return "INSTANT";
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

            default:
                return "unknown token";
        }
    }


} // end of namespace