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
#include "Velox/Interpreter/InterpreterSimpleName.h"
#include "Velox/Interpreter/InterpreterFieldAccess.h"
#include "Velox/Interpreter/InterpreterAssignment.h"
#include "Velox/Interpreter/InterpreterIntegerLiteral.h"
#include "Velox/Interpreter/InterpreterFunctionCall.h"
#include "Velox/Interpreter/InterpreterActualParameterList.h"

// C++ Standard Library includes
#include <algorithm>

#define CONTAINS(a, b) (std::find(std::begin(a), std::end(a), b) != std::end(a))

using namespace std;

namespace velox {

    const TokenType Parser::STATEMENT_START[] = {TokenType::IDENT};
    const TokenType Parser::LVALUE_CHAIN[] = {TokenType::MEMBER_ACCESS, TokenType::LEFT_PARENTHESIS};
    //const TokenType Parser::TERM_CHAIN[] = {TokenType::ADD, TokenType::SUB};

    const std::map<TokenType, ArithmeticOperator> Parser::OP_TO_TOK = {
            {TokenType::ADD, ArithmeticOperator::ADD},
            {TokenType::SUB, ArithmeticOperator::SUB},
            {TokenType::MUL, ArithmeticOperator::MUL},
            {TokenType::DIV, ArithmeticOperator::DIV},
            {TokenType::MOD, ArithmeticOperator::MOD},
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

            //case TokenType::WHILE:
            //    break;

            default:
                throw ParserError("Internal parser error: unexpected token while parsing statement");
        }

        return nullptr;
    }

    std::shared_ptr<InterpreterStatement> Parser::ParseIdentStatement(Source &source) {
        // FIXME true is not a parameter but a result of the parsing.
        auto lValue = ParseVariable(source, true);

        switch (source.GetCurrentTokenType()) {
            case TokenType::ASSIGNMENT:
                return ParseAssignment(source, lValue);

            default:
                throw ParserError("Syntax Error");
        }

        //return std::shared_ptr<InterpreterStatement>();
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
        std::shared_ptr<InterpreterExpression> result = ParseSimpleName(source, true);

        while (CONTAINS(LVALUE_CHAIN, source.GetCurrentTokenType())) {
            switch (source.GetCurrentTokenType()) {
                case TokenType::MEMBER_ACCESS:
                    result = ParseMemberAccess(source, location, result);
                    break;

                case TokenType::LEFT_PARENTHESIS:
                    if (location) {
                        throw ParserError("l-value expected");
                    }
                    result = ParseFunctionCall(source, result);
                    break;
            }
        }

        return result;
    }

    std::shared_ptr<InterpreterExpression>
    Parser::ParseMemberAccess(Source &source, bool location, std::shared_ptr<InterpreterExpression> lValue) {
        assert(source.GetCurrentTokenType() == TokenType::MEMBER_ACCESS);
        if (source.GetNextTokenType() != TokenType::IDENT) {
            throw ParserError("Field name expected");
        }

        auto result = make_shared<InterpreterFieldAccess>(location);
        result->SetLeftHandSide(lValue);
        result->SetRightHandSide(source.GetStringValue());
        source.GetNextTokenType();

        return result;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseSimpleName(Source &source, bool location) {
        assert(source.GetCurrentTokenType() == TokenType::IDENT);
        auto result = make_shared<InterpreterSimpleName>(source.GetStringValue(), location);
        source.GetNextTokenType();
        return result;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseRightValue(Source &source) {
        return ParseSimpleExpression(source);
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseSimpleExpression(Source &source)  {
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
               source.GetCurrentTokenType() == TokenType::MOD)
        {
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

    std::shared_ptr<InterpreterExpression> Parser::ParseFunctionCall(Source &source, std::shared_ptr<InterpreterExpression> function) {
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

}

