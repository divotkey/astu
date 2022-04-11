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
#include "Velox/Interpreter/InterpreterAssignmentOperator.h"
#include "Velox/Interpreter/InterpreterRelationalOperation.h"
#include "Velox/Interpreter/InterpreterPreIncrement.h"
#include "Velox/Interpreter/InterpreterPostIncrement.h"
#include "Velox/Interpreter/InterpreterLogicalOperation.h"
#include "Velox/Interpreter/InterpreterSimpleName.h"
#include "Velox/Interpreter/InterpreterMemberAccess.h"
#include "Velox/Interpreter/InterpreterAssignment.h"
#include "Velox/Interpreter/InterpreterIntegerLiteral.h"
#include "Velox/Interpreter/InterpreterRealLiteral.h"
#include "Velox/Interpreter/InterpreterStringLiteral.h"
#include "Velox/Interpreter/InterpreterFunctionCall.h"
#include "Velox/Interpreter/InterpreterScriptFunction.h"
#include "Velox/Interpreter/InterpreterReturnStatement.h"
#include "Velox/Interpreter/InterpreterIfStatement.h"
#include "Velox/Interpreter/InterpreterWhileStatement.h"
#include "Velox/Interpreter/InterpreterClassDefinition.h"
#include "Velox/Interpreter/InterpreterNewStatement.h"

// DEBUG
#include <iostream>

// C++ Standard Library includes
#include <algorithm>

#define CONTAINS(a, b) (std::find(std::begin(a), std::end(a), b) != std::end(a))

using namespace std;

namespace velox {

    const TokenType Parser::STATEMENT_START[] = {TokenType::IDENT, TokenType::INTEGER, TokenType::REAL,
                                                 TokenType::STRING, TokenType::DECREMENT, TokenType::INCREMENT,
                                                 TokenType::FUNCTION, TokenType::RETURN,
                                                 TokenType::IF, TokenType::WHILE, TokenType::CLASS, TokenType::NEW,

    };

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

    void Parser::ParseRightParenthesis(Source &source) {
        if (source.GetCurrentTokenType() != TokenType::RIGHT_PARENTHESIS) {
            throw ParserError("')' expected", source.GetLineNumber());
        }
        source.GetNextTokenType();
    }

    void Parser::ParseLeftParenthesis(Source &source) {
        if (source.GetCurrentTokenType() != TokenType::LEFT_PARENTHESIS) {
            throw ParserError("'(' expected", source.GetLineNumber());
        }
        source.GetNextTokenType();
    }

    void Parser::ParseBlockStart(Source &source) {
        if (source.GetCurrentTokenType() != TokenType::BLOCK_START) {
            throw ParserError("'{' expected", source.GetLineNumber());
        }
        source.GetNextTokenType();
    }

    void Parser::ParseBlockEnd(Source &source) {
        if (source.GetCurrentTokenType() != TokenType::BLOCK_END) {
            throw ParserError("'}' expected", source.GetLineNumber());
        }
        source.GetNextTokenType();

    }

    void Parser::ParseSemicolon(Source &source) {
        if (source.GetCurrentTokenType() != TokenType::SEMICOLON) {
            throw ParserError("';' expected", source.GetLineNumber());
        }
        source.GetNextTokenType();
    }

    std::shared_ptr<InterpreterStatementBlock> Parser::Parse(Source &source) {
        source.GetNextTokenType();

        std::shared_ptr<InterpreterStatementBlock> result;
        try {
            result = ParseStatementBlock(source);
        } catch (const astu::ScannerException &e) {
            throw ParserError(e.what(), source.GetLineNumber());
        }

        if (source.GetCurrentTokenType() != TokenType::EOS) {
            throw ParserError("end of source code expected", source.GetLineNumber());
        }

        return result;
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

        std::shared_ptr<InterpreterStatement> result;

        switch (source.GetCurrentTokenType()) {

            case TokenType::INTEGER:
                // Fall through

            case TokenType::REAL:
                // Fall through

            case TokenType::STRING:
                // Fall through

            case TokenType::INCREMENT:
                // Fall through

            case TokenType::DECREMENT:
                // Fall through

            case TokenType::NEW:
                // Fall through

            case TokenType::IDENT:
                result = ParseOptionalAssignment(source, ParseSimpleExpression(source));
                ParseSemicolon(source);
                break;

            case TokenType::FUNCTION:
                result = ParseFunctionDefinition(source);
                break;

            case TokenType::RETURN:
                result = ParseReturnStatement(source);
                ParseSemicolon(source);
                break;

            case TokenType::IF:
                result = ParseIfStatement(source);
                break;

            case TokenType::WHILE:
                result = ParseWhileStatement(source);
                break;

            case TokenType::CLASS:
                result = ParseClassDefinition(source);
                break;

            default:
                throw logic_error("Internal parser error: unexpected token while parsing statement");
        }

        return result;
    }

    std::shared_ptr<InterpreterStatement>
    Parser::ParseOptionalAssignment(Source &source, std::shared_ptr<InterpreterExpression> lValue) {

        switch (source.GetCurrentTokenType()) {
            case TokenType::ASSIGNMENT:
                return ParseAssignment(source, lValue);

            case TokenType::ASSIGN_ADD:
                return ParseAssignmentOperator(source, lValue, ArithmeticOperator::ADD);

            case TokenType::ASSIGN_SUB:
                return ParseAssignmentOperator(source, lValue, ArithmeticOperator::SUB);

            case TokenType::ASSIGN_MUL:
                return ParseAssignmentOperator(source, lValue, ArithmeticOperator::MUL);

            case TokenType::ASSIGN_DIV:
                return ParseAssignmentOperator(source, lValue, ArithmeticOperator::DIV);

            case TokenType::ASSIGN_MOD:
                return ParseAssignmentOperator(source, lValue, ArithmeticOperator::MOD);

            default:
                return lValue;
        }
    }


    std::shared_ptr<InterpreterStatement>
    Parser::ParseAssignment(Source &source, std::shared_ptr<InterpreterExpression> lValue) {
        // Verify expression is assignable.
        if (!lValue->IsLocation()) {
            throw ParserError("lvalue required as left operand of assignment", source.GetLineNumber());
        }

        // Eat assignment operator.
        source.GetNextTokenType();

        // Assemble assignment.
        auto result = make_shared<InterpreterAssignment>();
        result->SetLeftHandSide(lValue);
        result->SetRightHandSide(ParseExpression(source));

        return result;
    }

    std::shared_ptr<InterpreterStatement>
    Parser::ParseAssignmentOperator(Source &source, std::shared_ptr<InterpreterExpression> lValue,
                                    ArithmeticOperator op)
    {
        // Verify expression is assignable.
        if (!lValue->IsLocation()) {
            throw ParserError("lvalue required as left operand of assignment", source.GetLineNumber());
        }

        // Eat assignment operator.
        source.GetNextTokenType();

        // Assemble assignment.
        auto result = make_shared<InterpreterAssignmentOperator>(op);
        result->SetLeftHandSide(lValue);
        result->SetRightHandSide(ParseExpression(source));

        return result;
    }

    std::shared_ptr<InterpreterExpression>
    Parser::ParseOptionalSelector(Source &source, std::shared_ptr<InterpreterExpression> lValue) {

        while (CONTAINS(LVALUE_CHAIN, source.GetCurrentTokenType())) {
            switch (source.GetCurrentTokenType()) {
                case TokenType::MEMBER_ACCESS:
                    return ParseOptionalSelector(source, ParseMemberAccess(source, lValue));

                case TokenType::LEFT_PARENTHESIS:
                    return ParseOptionalSelector(source, ParseFunctionCall(source, lValue));
            }
        }

        return lValue;
    }

    std::shared_ptr<InterpreterExpression>
    Parser::ParseMemberAccess(Source &source, std::shared_ptr<InterpreterExpression> lValue) {
        assert(source.GetCurrentTokenType() == TokenType::MEMBER_ACCESS);

        source.GetNextTokenType();
        if (source.GetCurrentTokenType() != TokenType::IDENT) {
            throw ParserError("Field name expected");
        }

        auto result = make_shared<InterpreterMemberAccess>(source.GetLineNumber());
        result->SetLeftHandSide(lValue);
        result->SetRightHandSide(source.GetStringValue());
        source.GetNextTokenType();

        return result;
    }

    std::shared_ptr<InterpreterSimpleName> Parser::ParseSimpleName(Source &source) {
        if (source.GetCurrentTokenType() != TokenType::IDENT) {
            throw ParserError("identifier expected", source.GetLineNumber());
        }
        auto result = make_shared<InterpreterSimpleName>(source.GetStringValue(), source.GetLineNumber());
        source.GetNextTokenType();
        return result;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseExpression(Source &source) {
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

            case TokenType::INCREMENT:
                source.GetNextTokenType();
                result = make_shared<InterpreterPreIncrement>(ParseFactor(source), false);
                break;

            case TokenType::DECREMENT:
                source.GetNextTokenType();
                result = make_shared<InterpreterPreIncrement>(ParseFactor(source), true);
                break;

            case TokenType::INTEGER:
                result = make_shared<InterpreterIntegerLiteral>(source.GetIntegerValue());
                source.GetNextTokenType();
                break;

            case TokenType::REAL:
                result = make_shared<InterpreterRealLiteral>(source.GetRealValue());
                source.GetNextTokenType();
                break;

            case TokenType::STRING:
                result = make_shared<InterpreterStringLiteral>(source.GetStringValue());
                source.GetNextTokenType();
                break;

            case TokenType::IDENT:
                result = ParseFactorIdent(source);
                break;

            case TokenType::NEW:
                result = ParseOptionalSelector(source, ParseNewStatement(source));
                break;

            case TokenType::LEFT_PARENTHESIS:
                source.GetNextTokenType();
                result = ParseExpression(source);
                ParseRightParenthesis(source);
                break;
        }

        return result;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseFactorIdent(Source &source) {
        auto result = ParseOptionalSelector(source, ParseSimpleName(source));

        switch (source.GetCurrentTokenType()) {
            case TokenType::INCREMENT:
                source.GetNextTokenType();
                result = make_shared<InterpreterPostIncrement>(result, false);
                break;

            case TokenType::DECREMENT:
                source.GetNextTokenType();
                result = make_shared<InterpreterPostIncrement>(result, true);
                break;
        }

        return result;
    }

    std::shared_ptr<InterpreterFunctionCall>
    Parser::ParseFunctionCall(Source &source, std::shared_ptr<InterpreterExpression> function) {
        assert(source.GetCurrentTokenType() == TokenType::LEFT_PARENTHESIS);

        auto result = make_shared<InterpreterFunctionCall>(source.GetLineNumber());
        result->SetFunction(function);

        source.GetNextTokenType();

        // Look if function call contains parameters.
        if (source.GetCurrentTokenType() != TokenType::RIGHT_PARENTHESIS) {
            result->AddParameter(ParseExpression(source));
            while (source.GetCurrentTokenType() == TokenType::COMMA) {
                source.GetNextTokenType();
                result->AddParameter(ParseExpression(source));
            }
        }

        ParseRightParenthesis(source);

        return result;
    }

    std::shared_ptr<InterpreterFunctionDefinition> Parser::ParseFunctionDefinition(Source &source) {
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
            result->SetReturnExpression(ParseExpression(source));
        }

        return result;
    }

    std::shared_ptr<InterpreterStatement> Parser::ParseIfStatement(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::IF);
        source.GetNextTokenType();

        auto result = make_shared<InterpreterIfStatement>();
        ParseLeftParenthesis(source);

        result->SetCondition(ParseExpression(source));
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

        result->SetCondition(ParseExpression(source));
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

    std::shared_ptr<InterpreterStatement> Parser::ParseClassDefinition(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::CLASS);
        source.GetNextTokenType();

        if (source.GetCurrentTokenType() != TokenType::IDENT) {
            throw ParserError("Identifier expected", source.GetLineNumber());
        }

        auto result = make_shared<InterpreterClassDefinition>(source.GetStringValue());
        source.GetNextTokenType();
        ParseBlockStart(source);

        while (source.GetCurrentTokenType() != TokenType::BLOCK_END) {
            auto function = ParseFunctionDefinition(source);
            if (result->HasFunction(function->GetFunctionName())) {
                throw ParserError("Ambiguous function name '" + function->GetFunctionName() + "'",
                                  source.GetLineNumber());
            }
            result->AddFunction(function);
        }

        ParseBlockEnd(source);
        return result;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseNewStatement(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::NEW);
        auto result = make_shared<InterpreterNewStatement>(source.GetLineNumber());
        source.GetNextTokenType();

        if (source.GetCurrentTokenType() != TokenType::IDENT) {
            throw ParserError("Identifier expected", source.GetLineNumber());
        }
        result->SetTypeName(source.GetStringValue());
        unsigned int identLineNumber = source.GetLineNumber();
        source.GetNextTokenType();

        result->SetConstructorCall(ParseConstructorCall(source));

        return result;
    }

    std::shared_ptr<InterpreterConstructorCall> Parser::ParseConstructorCall(Source &source) {
        if (source.GetCurrentTokenType() != TokenType::LEFT_PARENTHESIS) {
            throw ParserError("'(' expected", source.GetLineNumber());
        }
        source.GetNextTokenType();

        auto result = make_shared<InterpreterConstructorCall>(source.GetLineNumber());

        // Look if function call contains parameters.
        if (source.GetCurrentTokenType() != TokenType::RIGHT_PARENTHESIS) {
            result->AddParameter(ParseExpression(source));
            while (source.GetCurrentTokenType() == TokenType::COMMA) {
                source.GetNextTokenType();
                result->AddParameter(ParseExpression(source));
            }
        }

        ParseRightParenthesis(source);
        return result;
    }

}
