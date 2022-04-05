// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Parser.h"
#include "ParserError.h"
#include "Script/ScannerException.h"
#include "Velox/Interpreter/InterpreterArithmeticOperation.h"
#include "Velox/Interpreter/InterpreterRelationalOperation.h"
#include "Velox/Interpreter/InterpreterLogicalOperation.h"
#include "Velox/Interpreter/InterpreterSimpleName.h"
#include "Velox/Interpreter/InterpreterMemberAccess.h"
#include "Velox/Interpreter/InterpreterAssignment.h"
#include "Velox/Interpreter/InterpreterIntegerLiteral.h"
#include "Velox/Interpreter/InterpreterRealLiteral.h"
#include "Velox/Interpreter/InterpreterFunctionCall.h"
#include "Velox/Interpreter/InterpreterFunctionDefinition.h"
#include "Velox/Interpreter/InterpreterScriptFunction.h"
#include "Velox/Interpreter/InterpreterReturnStatement.h"
#include "Velox/Interpreter/InterpreterIfStatement.h"
#include "Velox/Interpreter/InterpreterWhileStatement.h"

// DEBUG
#include <iostream>

// C++ Standard Library includes
#include <algorithm>

#define CONTAINS(a, b) (std::find(std::begin(a), std::end(a), b) != std::end(a))

using namespace std;

namespace velox {

    const TokenType Parser::STATEMENT_START[] = {TokenType::IDENT, TokenType::FUNCTION, TokenType::RETURN,
                                                 TokenType::IF, TokenType::WHILE};

    const TokenType Parser::LVALUE_CHAIN[] = {TokenType::MEMBER_ACCESS, TokenType::LEFT_PARENTHESIS};
    const TokenType Parser::REL_OP[] = {TokenType::EQUAL, TokenType::NOT_EQUAL, TokenType::LESS_EQUAL,
                                        TokenType::LESS_THAN, TokenType::GREATER_EQUAL, TokenType::GREATER_THAN};

    const std::map<TokenType, ArithmeticOperator> Parser::OP_TO_TOK = {
            {TokenType::ADD, ArithmeticOperator::ADD},
            {TokenType::SUB, ArithmeticOperator::SUB},
            {TokenType::MUL, ArithmeticOperator::MUL},
            {TokenType::DIV, ArithmeticOperator::DIV},
            {TokenType::MOD, ArithmeticOperator::MOD},
    };

    const std::map<TokenType, RelationalOperator> Parser::REL_OP_TO_TOK = {
            {TokenType::EQUAL,         RelationalOperator::EQUAL},
            {TokenType::NOT_EQUAL,     RelationalOperator::NOT_EQUAL},
            {TokenType::LESS_THAN,     RelationalOperator::LESS_THAN},
            {TokenType::LESS_EQUAL,    RelationalOperator::LESS_EQUAL},
            {TokenType::GREATER_THAN,  RelationalOperator::GREATER_THAN},
            {TokenType::GREATER_EQUAL, RelationalOperator::GREATER_EQUAL}
    };

    const std::map<TokenType, LogicalOperator> Parser::LOG_OP_TO_TOK = {
            {TokenType::LOG_AND,         LogicalOperator::AND},
            {TokenType::LOG_OR,     LogicalOperator::OR}
    };

    std::shared_ptr<InterpreterStatementBlock> Parser::Parse(Source &source) {
        source.GetNextTokenType();

        std::shared_ptr<InterpreterStatementBlock> result;
        try {
            result = ParseStatementBlock(source);
        } catch (const astu::ScannerException &e) {
            throw ParserError(e.what());
        }

        if (source.GetCurrentTokenType() != TokenType::EOS) {
            throw ParserError("End of source code expected");
        }

        return result;
    }

    void Parser::ParseRightParenthesis(Source &source) {
        if (source.GetCurrentTokenType() != TokenType::RIGHT_PARENTHESIS) {
            throw ParserError("Syntax error: ')' expected");
        }
        source.GetNextTokenType();
    }

    void Parser::ParseLeftParenthesis(Source &source) {
        if (source.GetCurrentTokenType() != TokenType::LEFT_PARENTHESIS) {
            throw ParserError("Syntax error: '(' expected");
        }
        source.GetNextTokenType();
    }

    void Parser::ParseBlockStart(Source &source) {
        if (source.GetCurrentTokenType() != TokenType::BLOCK_START) {
            throw ParserError("Syntax error: '{' expected");
        }
        source.GetNextTokenType();
    }

    void Parser::ParseBlockEnd(Source &source) {
        if (source.GetCurrentTokenType() != TokenType::BLOCK_END) {
            throw ParserError("Syntax error: '}' expected");
        }
        source.GetNextTokenType();

    }

    void Parser::ParseSemicolon(Source &source) {
        if (source.GetCurrentTokenType() != TokenType::SEMICOLON) {
            throw ParserError("Syntax error: ';' expected");
        }
        source.GetNextTokenType();
    }

    std::shared_ptr<InterpreterStatementBlock> Parser::ParseStatementBlock(Source &source) {

        auto result = make_shared<InterpreterStatementBlock>();

        while (CONTAINS(STATEMENT_START, source.GetCurrentTokenType())) {
            auto statement = ParseStatement(source);
            result->AddStatement(statement);
        }

        return result;
    }

    std::shared_ptr<InterpreterStatement> Parser::ParseStatement(Source &source) {
        assert(CONTAINS(STATEMENT_START, source.GetCurrentTokenType()));

        switch (source.GetCurrentTokenType()) {

            case TokenType::IDENT:
                return ParseIdentStatement(source);

            case TokenType::FUNCTION:
                return ParseFunctionDefinition(source);

            case TokenType::RETURN:
                return ParseReturnStatement(source);

            case TokenType::IF:
                return ParseIfStatement(source);

            case TokenType::WHILE:
                return ParseWhileStatement(source);

            default:
                throw ParserError("Internal parser error: unexpected token while parsing statement");
        }

        return nullptr;
    }

    std::shared_ptr<InterpreterStatement> Parser::ParseIdentStatement(Source &source) {
        auto result = ParseVariable(source, true);

        switch (source.GetCurrentTokenType()) {
            case TokenType::ASSIGNMENT:
                return ParseAssignment(source, result);

            default:
                ParseSemicolon(source);
                return result;
        }
    }

    std::shared_ptr<InterpreterStatement>
    Parser::ParseAssignment(Source &source, std::shared_ptr<InterpreterExpression> lValue) {
        assert(source.GetCurrentTokenType() == TokenType::ASSIGNMENT);
        assert(lValue->IsLocation());

        source.GetNextTokenType();
        auto rValue = ParseRightValue(source);

        ParseSemicolon(source);
        auto result = make_shared<InterpreterAssignment>();
        result->SetLeftHandSide(lValue);
        result->SetRightHandSide(rValue);

        return result;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseVariable(Source &source, bool location) {
        std::shared_ptr<InterpreterExpression> result = ParseSimpleName(source);

        while (CONTAINS(LVALUE_CHAIN, source.GetCurrentTokenType())) {
            switch (source.GetCurrentTokenType()) {
                case TokenType::MEMBER_ACCESS:
                    result = ParseMemberAccess(source, result);
                    break;

                case TokenType::LEFT_PARENTHESIS:
                    result->SetLocation(false);
                    result = ParseFunctionCall(source, result);
                    break;
            }
        }

        return result;
    }

    std::shared_ptr<InterpreterExpression>
    Parser::ParseMemberAccess(Source &source, std::shared_ptr<InterpreterExpression> lValue) {
        assert(source.GetCurrentTokenType() == TokenType::MEMBER_ACCESS);
        source.GetNextTokenType();
        if (source.GetCurrentTokenType() != TokenType::IDENT) {
            throw ParserError("Field name expected");
        }

        auto result = make_shared<InterpreterMemberAccess>();
        result->SetLeftHandSide(lValue);
        result->SetRightHandSide(source.GetStringValue());
        source.GetNextTokenType();

        return result;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseSimpleName(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::IDENT);
        auto result = make_shared<InterpreterSimpleName>(source.GetStringValue(), true);
        source.GetNextTokenType();
        return result;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseRightValue(Source &source) {
        auto expr = ParseOrExpression(source);

        while (source.GetCurrentTokenType() == TokenType::LOG_AND) {
            auto logOp = make_shared<InterpreterLogicalOperation>(
                    LOG_OP_TO_TOK.at(source.GetCurrentTokenType())
            );

            logOp->SetLeftHandSide(expr);
            expr = logOp;

            source.GetNextTokenType();
            logOp->SetRightHandSide(ParseOrExpression(source));
        }

        return expr;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseOrExpression(Source &source) {
        auto expr = ParseRelExpression(source);

        while (source.GetCurrentTokenType() == TokenType::LOG_OR) {
            auto logOp = make_shared<InterpreterLogicalOperation>(
                    LOG_OP_TO_TOK.at(source.GetCurrentTokenType())
            );

            logOp->SetLeftHandSide(expr);
            expr = logOp;

            source.GetNextTokenType();
            logOp->SetRightHandSide(ParseRelExpression(source));
        }

        return expr;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseRelExpression(Source &source) {
        auto expr = ParseSimpleExpression(source);

        while (CONTAINS(REL_OP, source.GetCurrentTokenType())) {
            auto relOp = make_shared<InterpreterRelationalOperation>(
                    REL_OP_TO_TOK.at(source.GetCurrentTokenType())
            );

            relOp->SetLeftHandSide(expr);
            expr = relOp;

            source.GetNextTokenType();
            relOp->SetRightHandSide(ParseSimpleExpression(source));
        }

        return expr;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseSimpleExpression(Source &source) {
        auto term = ParseTerm(source);

        while (source.GetCurrentTokenType() == TokenType::ADD || source.GetCurrentTokenType() == TokenType::SUB) {
            auto arithOp
                    = make_shared<InterpreterArithmeticOperation>(OP_TO_TOK.at(source.GetCurrentTokenType()));
            arithOp->SetLeftHandSide(term);
            term = arithOp;

            source.GetNextTokenType();
            arithOp->SetRightHandSide(ParseTerm(source));
        }

        return term;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseTerm(Source &source) {
        auto factor = ParseFactor(source);

        while (source.GetCurrentTokenType() == TokenType::MUL || source.GetCurrentTokenType() == TokenType::DIV ||
               source.GetCurrentTokenType() == TokenType::MOD) {
            auto arithOp
                    = make_shared<InterpreterArithmeticOperation>(OP_TO_TOK.at(source.GetCurrentTokenType()));

            arithOp->SetLeftHandSide(factor);
            factor = arithOp;

            source.GetNextTokenType();
            arithOp->SetRightHandSide(ParseFactor(source));
        }

        return factor;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseFactor(Source &source) {

        // Ignore unary plus.
        if (source.GetCurrentTokenType() == TokenType::ADD) {
            source.GetNextTokenType();
        }

        std::shared_ptr<InterpreterExpression> result;

        switch (source.GetCurrentTokenType()) {
            case TokenType::SUB:
                throw std::runtime_error("Unary minus not implemented");

            case TokenType::INTEGER:
                result = make_shared<InterpreterIntegerLiteral>(source.GetIntegerValue());
                source.GetNextTokenType();
                break;

            case TokenType::REAL:
                result = make_shared<InterpreterRealLiteral>(source.GetRealValue());
                source.GetNextTokenType();
                break;

            case TokenType::IDENT:
                result = ParseFactorIdent(source);
                break;

            case TokenType::LEFT_PARENTHESIS:
                source.GetNextTokenType();
                result = ParseRightValue(source);
                ParseRightParenthesis(source);
                break;
        }

        return result;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseFactorIdent(Source &source) {
        auto result = ParseVariable(source, false);
        // TODO parse optional post-fix operation (++ or --).

        return result;
    }

    std::shared_ptr<InterpreterExpression>
    Parser::ParseFunctionCall(Source &source, std::shared_ptr<InterpreterExpression> function) {
        assert(source.GetCurrentTokenType() == TokenType::LEFT_PARENTHESIS);
        source.GetNextTokenType();

        auto result = make_shared<InterpreterFunctionCall>();
        result->SetFunction(function);

        // Look if function call contains parameters.
        if (source.GetCurrentTokenType() != TokenType::RIGHT_PARENTHESIS) {
            result->AddParameter(ParseRightValue(source));
            while (source.GetCurrentTokenType() == TokenType::COMMA) {
                source.GetNextTokenType();
                result->AddParameter(ParseRightValue(source));
            }
        }

        ParseRightParenthesis(source);

        return result;
    }

    std::shared_ptr<InterpreterStatement> Parser::ParseFunctionDefinition(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::FUNCTION);
        source.GetNextTokenType();

        if (source.GetCurrentTokenType() != TokenType::IDENT) {
            throw ParserError("Function name expected");
        }

        auto result = make_shared<InterpreterFunctionDefinition>();
        result->SetFunctionName(source.GetStringValue());
        source.GetNextTokenType();

        ParseLeftParenthesis(source);

        auto function = make_shared<InterpreterScriptFunction>();

        if (source.GetCurrentTokenType() != TokenType::RIGHT_PARENTHESIS) {

            bool first = true;
            do {
                if (!first) {
                    // Eat comma.
                    source.GetNextTokenType();
                }
                first = false;

                if (source.GetCurrentTokenType() != TokenType::IDENT) {
                    throw ParserError("Parameter name expected");
                }
                function->AddFormalParameter(source.GetStringValue());
                source.GetNextTokenType();
            } while (source.GetCurrentTokenType() == TokenType::COMMA);
        }

        ParseRightParenthesis(source);
        ParseBlockStart(source);
        function->SetStatement(ParseStatementBlock(source));
        ParseBlockEnd(source);

        result->SetFunction(function);
        return result;
    }

    std::shared_ptr<InterpreterStatement> Parser::ParseReturnStatement(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::RETURN);
        source.GetNextTokenType();

        auto result = make_shared<InterpreterReturnStatement>();
        if (source.GetCurrentTokenType() != TokenType::SEMICOLON) {
            result->SetReturnExpression(ParseRightValue(source));
        }

        ParseSemicolon(source);

        return result;
    }

    std::shared_ptr<InterpreterStatement> Parser::ParseIfStatement(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::IF);
        source.GetNextTokenType();

        auto result = make_shared<InterpreterIfStatement>();
        ParseLeftParenthesis(source);

        result->SetCondition(ParseRightValue(source));
        ParseRightParenthesis(source);
        if (source.GetCurrentTokenType() == TokenType::BLOCK_START) {
            source.GetNextTokenType();
            result->SetThenPart(ParseStatementBlock(source));
            ParseBlockEnd(source);
        } else {
            result->SetThenPart(ParseStatement(source));
        }

        if (source.GetCurrentTokenType() == TokenType::ELSE) {
            source.GetNextTokenType();
            if (source.GetCurrentTokenType() == TokenType::BLOCK_START) {
                source.GetNextTokenType();
                result->SetElsePart(ParseStatementBlock(source));
                ParseBlockEnd(source);
            } else {
                result->SetElsePart(ParseStatement(source));
            }
        }

        return result;
    }

    std::shared_ptr<InterpreterStatement> Parser::ParseWhileStatement(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::WHILE);
        source.GetNextTokenType();

        auto result = make_shared<InterpreterWhileStatement>();
        ParseLeftParenthesis(source);

        result->SetCondition(ParseRightValue(source));
        ParseRightParenthesis(source);
        if (source.GetCurrentTokenType() == TokenType::BLOCK_START) {
            source.GetNextTokenType();
            result->SetStatement(ParseStatementBlock(source));
            ParseBlockEnd(source);
        } else {
            result->SetStatement(ParseStatement(source));
        }

        return result;
    }

}
