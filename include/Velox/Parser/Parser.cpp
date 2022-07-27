// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Parser.h"
#include "ParserError.h"
#include "Script/ScannerError.h"
#include "Velox/Interpreter/InterpreterArithmeticOperation.h"
#include "Velox/Interpreter/InterpreterAssignmentOperator.h"
#include "Velox/Interpreter/InterpreterRelationalOperation.h"
#include "Velox/Interpreter/InterpreterPreIncrement.h"
#include "Velox/Interpreter/InterpreterPostIncrement.h"
#include "Velox/Interpreter/InterpreterLogicalOperation.h"
#include "Velox/Interpreter/InterpreterExpressionSimpleName.h"
#include "Velox/Interpreter/InterpreterExpressionMemberAccess.h"
#include "Velox/Interpreter/InterpreterExpressionListAccess.h"
#include "Velox/Interpreter/InterpreterExpressionAssignment.h"
#include "Velox/Interpreter/InterpreterLiteralInteger.h"
#include "Velox/Interpreter/InterpreterLiteralReal.h"
#include "Velox/Interpreter/InterpreterLiteralBoolean.h"
#include "Velox/Interpreter/InterpreterLiteralString.h"
#include "Velox/Interpreter/InterpreterLiteralList.h"
#include "Velox/Interpreter/InterpreterExpressionUndefined.h"
#include "Velox/Interpreter/InterpreterColor.h"
#include "Velox/Interpreter/InterpreterVector.h"
#include "Velox/Interpreter/InterpreterFunctionCall.h"
#include "Velox/Interpreter/InterpreterFunctionScript.h"
#include "Velox/Interpreter/InterpreterReturnStatement.h"
#include "Velox/Interpreter/InterpreterStatementGlobal.h"
#include "Velox/Interpreter/InterpreterStatementIf.h"
#include "Velox/Interpreter/InterpreterStatementWhile.h"
#include "Velox/Interpreter/InterpreterStatementDoWhile.h"
#include "Velox/Interpreter/InterpreterStatementLoop.h"
#include "Velox/Interpreter/InterpreterStatementFor.h"
#include "Velox/Interpreter/InterpreterStatementBreak.h"
#include "Velox/Interpreter/InterpreterStatementContinue.h"
#include "Velox/Interpreter/InterpreterStatementNop.h"
#include "Velox/Interpreter/InterpreterClassDefinition.h"
#include "Velox/Interpreter/InterpreterInstantDefinition.h"
#include "Velox/Interpreter/InterpreterInstantRealization.h"
#include "Velox/Interpreter/InterpreterExpressionUnaryMinus.h"
#include "Velox/Interpreter/InterpreterExpressionNew.h"

// C++ Standard Library includes
#include <algorithm>

#define CONTAINS(a, b) (std::find(std::begin(a), std::end(a), b) != std::end(a))
#define OPERATOR_NAME   "operator"

using namespace std;

namespace velox {

    const TokenType Parser::STATEMENT_START[] = {TokenType::IDENT, TokenType::INTEGER, TokenType::REAL,
                                                 TokenType::STRING, TokenType::DECREMENT, TokenType::INCREMENT,
                                                 TokenType::FUNCTION, TokenType::RETURN, TokenType::GLOBAL,
                                                 TokenType::IF, TokenType::WHILE, TokenType::DO, TokenType::FOR,
                                                 TokenType::LOOP, TokenType::BREAK, TokenType::CONTINUE,
                                                 TokenType::CLASS, TokenType::NEW, TokenType::INSTANT
    };

    const TokenType Parser::LVALUE_CHAIN[] = {TokenType::MEMBER_ACCESS, TokenType::LEFT_BRACKET,
                                              TokenType::LEFT_PARENTHESIS};
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

    void Parser::ParseOptionalSemicolon(Source &source) {
        if (source.GetCurrentTokenType() == TokenType::SEMICOLON) {
            source.GetNextTokenType();
        }
    }

    void Parser::ParseComma(Source &source) {
        if (source.GetCurrentTokenType() != TokenType::COMMA) {
            throw ParserError("',' expected", source.GetLineNumber());
        }
        source.GetNextTokenType();
    }

    std::shared_ptr<InterpreterStatementBlock> Parser::Parse(Source &source) {

        std::shared_ptr<InterpreterStatementBlock> result;
        try {
            source.GetNextTokenType();
            result = ParseStatementBlock(source, false);
        } catch (const astu::ScannerError &e) {
            throw ParserError(e.what(), e.GetLineNumber());
        }

        if (source.GetCurrentTokenType() != TokenType::EOS) {
            throw ParserError("end of source code expected", source.GetLineNumber());
        }

        return result;
    }

    std::shared_ptr<InterpreterStatementBlock> Parser::ParseStatementBlock(Source &source, bool loopBody) {

        auto result = make_shared<InterpreterStatementBlock>(loopBody);

        while (CONTAINS(STATEMENT_START, source.GetCurrentTokenType())) {
            if (source.GetCurrentTokenType() == TokenType::SEMICOLON)
                continue;

            auto statement = ParseStatement(source);
            result->AddStatement(statement);
        }

        return result;
    }

    std::shared_ptr<InterpreterStatement> Parser::ParseForHeaderStatement(Source &source) {
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
                result = ParseOptionalAssignment(source, ParseExpression(source));
                break;

            default:
                throw ParserError("invalid statement as for-loop initializes", source.GetLineNumber());
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

                if (source.PeekNextTokenType() == TokenType::BLOCK_START) {
                    result = ParseInstantRealization(source);
                } else {
                    result = ParseOptionalAssignment(source, ParseExpression(source));
                    ParseSemicolon(source);
                }
                break;

            case TokenType::FUNCTION:
                result = ParseFunctionDefinition(source);
                break;

            case TokenType::RETURN:
                result = ParseReturnStatement(source);
                ParseSemicolon(source);
                break;

            case TokenType::GLOBAL:
                result = ParseGlobalStatement(source);
                ParseSemicolon(source);
                break;
                break;

            case TokenType::IF:
                result = ParseIfStatement(source);
                break;

            case TokenType::WHILE:
                result = ParseWhileStatement(source);
                break;

            case TokenType::DO:
                result = ParseDoWhileStatement(source);
                ParseOptionalSemicolon(source);
                break;

            case TokenType::FOR:
                result = ParseForStatement(source);
                break;

            case TokenType::LOOP:
                result = ParseLoopStatement(source);
                break;

            case TokenType::BREAK:
                result = make_shared<InterpreterStatementBreak>(source.GetLineNumber());
                source.GetNextTokenType();
                ParseSemicolon(source);
                break;

            case TokenType::CONTINUE:
                result = make_shared<InterpreterStatementContinue>(source.GetLineNumber());
                source.GetNextTokenType();
                ParseSemicolon(source);
                break;

            case TokenType::CLASS:
                result = ParseClassDefinition(source);
                break;

            case TokenType::INSTANT:
                result = ParseInstantDefinition(source);
                break;

            default:
                throw logic_error("Internal parser error: unexpected token while parsing loopBody");
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

        // Assemble assignment.
        auto result = make_shared<InterpreterExpressionAssignment>(source.GetLineNumber());
        source.GetNextTokenType();
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
                    lValue->SetLocation(false);
                    return ParseOptionalSelector(source, ParseMemberAccess(source, lValue));

                case TokenType::LEFT_BRACKET:
                    lValue->SetLocation(false);
                    return ParseOptionalSelector(source, ParseListAccess(source, lValue));

                case TokenType::LEFT_PARENTHESIS:
                    lValue->SetLocation(false);
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
            throw ParserError("Field name expected", source.GetLineNumber());
        }

        auto result = make_shared<InterpreterExpressionMemberAccess>(source.GetLineNumber());
        result->SetLeftHandSide(lValue);
        result->SetRightHandSide(source.GetStringValue());
        source.GetNextTokenType();

        return result;
    }

    std::shared_ptr<InterpreterExpression>
    Parser::ParseListAccess(Source &source, std::shared_ptr<InterpreterExpression> lValue) {
        assert(source.GetCurrentTokenType() == TokenType::LEFT_BRACKET);
        auto result = make_shared<InterpreterExpressionListAccess>(source.GetLineNumber());
        source.GetNextTokenType();

        result->SetLeftHandSide(lValue);
        result->SetIndex(ParseExpression(source));
        if (source.GetCurrentTokenType() != TokenType::RIGHT_BRACKET) {
            throw ParserError("']' expected", source.GetLineNumber());
        }
        source.GetNextTokenType();

        return result;
    }

    std::shared_ptr<InterpreterExpressionSimpleName> Parser::ParseSimpleName(Source &source) {
        if (source.GetCurrentTokenType() != TokenType::IDENT) {
            throw ParserError("identifier expected", source.GetLineNumber());
        }
        auto result = make_shared<InterpreterExpressionSimpleName>(source.GetStringValue(), source.GetLineNumber());
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
                    REL_OP_TO_TOK.at(source.GetCurrentTokenType()), source.GetLineNumber()
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
                    = make_shared<InterpreterArithmeticOperation>(OP_TO_TOK.at(source.GetCurrentTokenType()),
                                                                  source.GetLineNumber());
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
                    = make_shared<InterpreterArithmeticOperation>(OP_TO_TOK.at(source.GetCurrentTokenType()),
                                                                  source.GetLineNumber());

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
        unsigned lineNumber;

        switch (source.GetCurrentTokenType()) {
            case TokenType::SUB:
                lineNumber = source.GetLineNumber();
                source.GetNextTokenType();
                result = make_shared<InterpreterExpressionUnaryMinus>(lineNumber, ParseFactor(source));
                break;

            case TokenType::INCREMENT:
                lineNumber = source.GetLineNumber();
                source.GetNextTokenType();
                result = make_shared<InterpreterPreIncrement>(ParseFactor(source), false, lineNumber);
                break;

            case TokenType::DECREMENT:
                lineNumber = source.GetLineNumber();
                source.GetNextTokenType();
                result = make_shared<InterpreterPreIncrement>(ParseFactor(source), true, lineNumber);
                break;

            case TokenType::INTEGER:
                result = make_shared<InterpreterLiteralInteger>(source.GetIntegerValue());
                source.GetNextTokenType();
                break;

            case TokenType::REAL:
                result = make_shared<InterpreterLiteralReal>(source.GetRealValue());
                source.GetNextTokenType();
                break;

            case TokenType::TRUE:
                result = make_shared<InterpreterLiteralBoolean>(true);
                source.GetNextTokenType();
                break;

            case TokenType::FALSE:
                result = make_shared<InterpreterLiteralBoolean>(false);
                source.GetNextTokenType();
                break;

            case TokenType::UNDEFINED:
                result = make_shared<InterpreterExpressionUndefined>(source.GetLineNumber());
                source.GetNextTokenType();
                break;

            case TokenType::STRING:
                result = make_shared<InterpreterLiteralString>(source.GetStringValue());
                source.GetNextTokenType();
                break;

            case TokenType::IDENT:
                result = ParseFactorIdent(source);
                break;

            case TokenType::NEW:
                result = ParseOptionalSelector(source, ParseNewStatement(source));
                break;

            case TokenType::BIN_OR:
                result = ParseColor(source);
                break;

            case TokenType::LESS_THAN:
                result = ParseVector(source);
                break;

            case TokenType::LEFT_BRACKET:
                result = ParseList(source);
                break;

            case TokenType::LEFT_PARENTHESIS:
                source.GetNextTokenType();
                result = ParseExpression(source);
                ParseRightParenthesis(source);
                break;

            default:
                throw ParserError("syntax error", source.GetLineNumber());
        }

        return result;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseFactorIdent(Source &source) {
        auto result = ParseOptionalSelector(source, ParseSimpleName(source));

        switch (source.GetCurrentTokenType()) {
            case TokenType::INCREMENT:
                result = make_shared<InterpreterPostIncrement>(result, false, source.GetLineNumber());
                source.GetNextTokenType();
                break;

            case TokenType::DECREMENT:
                result = make_shared<InterpreterPostIncrement>(result, true, source.GetLineNumber());
                source.GetNextTokenType();
                break;
        }

        return result;
    }

    std::shared_ptr<InterpreterFunctionCall>
    Parser::ParseFunctionCall(Source &source, std::shared_ptr<InterpreterExpression> function) {
        assert(source.GetCurrentTokenType() == TokenType::LEFT_PARENTHESIS);
        function->SetLocation(false);

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

        if (result->GetFunctionName() == OPERATOR_NAME) {
            switch (source.GetCurrentTokenType()) {
                case TokenType::ADD:
                    result->SetFunctionName(result->GetFunctionName() + '+');
                    source.GetNextTokenType();
                    break;

                case TokenType::SUB:
                    result->SetFunctionName(result->GetFunctionName() + '-');
                    source.GetNextTokenType();
                    break;

                case TokenType::MUL:
                    result->SetFunctionName(result->GetFunctionName() + '*');
                    source.GetNextTokenType();
                    break;

                case TokenType::DIV:
                    result->SetFunctionName(result->GetFunctionName() + '/');
                    source.GetNextTokenType();
                    break;

                case TokenType::MOD:
                    result->SetFunctionName(result->GetFunctionName() + '%');
                    source.GetNextTokenType();
                    break;
            }
        }

        ParseLeftParenthesis(source);

        auto function = make_shared<InterpreterFunctionScript>();

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
        function->SetStatement(ParseStatementBlock(source, false));
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

    std::shared_ptr<InterpreterStatement> Parser::ParseGlobalStatement(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::GLOBAL);

        auto result = make_shared<InterpreterStatementGlobal>(source.GetLineNumber());
        source.GetNextTokenType();
        if (source.GetCurrentTokenType() != TokenType::IDENT) {
            throw ParserError("identifier expected", source.GetLineNumber());
        }

        result->SetName(source.GetStringValue());
        source.GetNextTokenType();
        return result;
    }


    std::shared_ptr<InterpreterStatement> Parser::ParseIfStatement(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::IF);
        source.GetNextTokenType();

        auto result = make_shared<InterpreterStatementIf>();
        ParseLeftParenthesis(source);

        result->SetCondition(ParseExpression(source));
        ParseRightParenthesis(source);
        result->SetThenPart(ParseStatementOrBlock(source, false));

        if (source.GetCurrentTokenType() == TokenType::ELSE) {
            source.GetNextTokenType();
            result->SetElsePart(ParseStatementOrBlock(source, false));
        }

        return result;
    }

    std::shared_ptr<InterpreterStatement> Parser::ParseWhileStatement(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::WHILE);

        auto result = make_shared<InterpreterStatementWhile>(source.GetLineNumber());
        source.GetNextTokenType();
        ParseLeftParenthesis(source);

        result->SetCondition(ParseExpression(source));
        ParseRightParenthesis(source);
        result->SetStatement(ParseStatementOrBlock(source, true));

        return result;
    }

    std::shared_ptr<InterpreterStatement> Parser::ParseDoWhileStatement(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::DO);

        auto result = make_shared<InterpreterStatementDoWhile>(source.GetLineNumber());
        source.GetNextTokenType();

        result->SetStatement(ParseStatementOrBlock(source, true));
        if (source.GetCurrentTokenType() != TokenType::WHILE) {
            throw ParserError("syntax error, 'while' expected", source.GetLineNumber());
        }
        source.GetNextTokenType();

        ParseLeftParenthesis(source);
        result->SetCondition(ParseExpression(source));
        ParseRightParenthesis(source);

        return result;
    }


    std::shared_ptr<InterpreterStatement> Parser::ParseLoopStatement(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::LOOP);

        auto result = make_shared<InterpreterStatementLoop>(source.GetLineNumber());
        source.GetNextTokenType();
        result->SetStatement(ParseStatementOrBlock(source, true));
        return result;
    }

    std::shared_ptr<InterpreterStatement> Parser::ParseForStatement(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::FOR);
        source.GetNextTokenType();

        auto result = make_shared<InterpreterStatementFor>();
        ParseLeftParenthesis(source);

        if (source.GetCurrentTokenType() == TokenType::SEMICOLON) {
            result->SetInitStatement(make_shared<InterpreterStatementNop>());
        } else {
            result->SetInitStatement(ParseForHeaderStatement(source));
        }
        ParseSemicolon(source);

        if (source.GetCurrentTokenType() == TokenType::SEMICOLON) {
            result->SetCondition(make_shared<InterpreterLiteralBoolean>(true));
        } else {
            result->SetCondition(ParseExpression(source));
        }
        ParseSemicolon(source);

        if (source.GetCurrentTokenType() == TokenType::SEMICOLON) {
            result->SetLoopStatement(make_shared<InterpreterStatementNop>());
        } else {
            result->SetLoopStatement(ParseForHeaderStatement(source));
        }

        ParseRightParenthesis(source);

        result->SetLoopBody(ParseStatementOrBlock(source, true));
        return result;
    }

    std::shared_ptr<InterpreterStatement> Parser::ParseClassDefinition(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::CLASS);
        source.GetNextTokenType();

        if (source.GetCurrentTokenType() != TokenType::IDENT) {
            throw ParserError("Identifier expected", source.GetLineNumber());
        }

        auto result = make_shared<InterpreterClassDefinition>(source.GetStringValue(), source.GetLineNumber());
        source.GetNextTokenType();
        ParseBlockStart(source);

        while (source.GetCurrentTokenType() != TokenType::BLOCK_END) {
            auto functionStart = source.GetLineNumber();
            auto function = ParseFunctionDefinition(source);
            if (result->HasFunction(function->GetFunctionName())) {
                throw ParserError("Ambiguous function name '" + function->GetFunctionName() + "'",
                                  functionStart);
            }
            result->AddFunction(function);
        }

        ParseBlockEnd(source);
        return result;
    }

    std::shared_ptr<InterpreterStatement> Parser::ParseInstantDefinition(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::INSTANT);
        source.GetNextTokenType();

        if (source.GetCurrentTokenType() != TokenType::IDENT) {
            throw ParserError("Identifier expected", source.GetLineNumber());
        }
        auto result = make_shared<InterpreterInstantDefinition>(source.GetStringValue(), source.GetLineNumber());
        source.GetNextTokenType();
        ParseBlockStart(source);

        while (source.GetCurrentTokenType() != TokenType::BLOCK_END) {
            auto functionStart = source.GetLineNumber();
            auto function = ParseFunctionDefinition(source);
            if (result->HasFunction(function->GetFunctionName())) {
                throw ParserError("Ambiguous function name '" + function->GetFunctionName() + "'",
                                  functionStart);
            }
            result->AddFunction(function);
        }

        ParseBlockEnd(source);
        return result;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseNewStatement(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::NEW);
        auto result = make_shared<InterpreterExpressionNew>(source.GetLineNumber());
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

    std::shared_ptr<InterpreterExpression> Parser::ParseColor(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::BIN_OR);
        auto lineNumber = source.GetLineNumber();
        source.GetNextTokenType();

        auto result = make_shared<InterpreterColor>();
        result->SetRedExpression(ParseSimpleExpression(source));
        if (source.GetCurrentTokenType() == TokenType::COMMA) {
            ParseComma(source);
            result->SetGreenExpression(ParseSimpleExpression(source));
            ParseComma(source);
            result->SetBlueExpression(ParseSimpleExpression(source));
            if (source.GetCurrentTokenType() == TokenType::COMMA) {
                source.GetNextTokenType();
                result->SetAlphaExpression(ParseSimpleExpression(source));
            }
        }

        if (source.GetCurrentTokenType() != TokenType::BIN_OR) {
            throw ParserError("'|' expected, got " + source.GetTokenTypeAsString(), source.GetLineNumber());
        }
        source.GetNextTokenType();

        return result;
    }

    double Parser::ParseReal(Source &source) {
        double result;
        switch (source.GetCurrentTokenType()) {
            case TokenType::INTEGER:
                result = source.GetIntegerValue();
                break;

            case TokenType::REAL:
                result = source.GetRealValue();
                break;

            default:
                throw ParserError("floating-point value expected");
        }

        source.GetNextTokenType();
        return result;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseVector(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::LESS_THAN);
        auto lineNumber = source.GetLineNumber();
        source.GetNextTokenType();

        auto result = make_shared<InterpreterVector>();
        result->SetXValueExpression(ParseSimpleExpression(source));
        ParseComma(source);
        result->SetYValueExpression(ParseSimpleExpression(source));
        if (source.GetCurrentTokenType() == TokenType::COMMA) {
            ParseComma(source);
            result->SetZValueExpression(ParseSimpleExpression(source));
        }

        if (source.GetCurrentTokenType() != TokenType::GREATER_THAN) {
            throw ParserError("'>' expected", source.GetLineNumber());
        }
        source.GetNextTokenType();

        return result;
    }

    std::shared_ptr<InterpreterStatement> Parser::ParseStatementOrBlock(Source &source, bool loopBody) {
        std::shared_ptr<InterpreterStatement> result;

        if (source.GetCurrentTokenType() == TokenType::BLOCK_START) {
            source.GetNextTokenType();
            result = ParseStatementBlock(source, loopBody);
            ParseBlockEnd(source);
        } else {
            result = ParseStatement(source);
        }

        return result;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseInstantRealization(Source &source) {
        assert (source.GetCurrentTokenType() == TokenType::IDENT);
        auto result = make_shared<InterpreterInstantRealization>(source.GetLineNumber());
        result->SetTypeName(source.GetStringValue());
        source.GetNextTokenType();

        ParseBlockStart(source);

        while (source.GetCurrentTokenType() != TokenType::BLOCK_END) {
            result->AddStatement(ParseStatement(source));
        }
        ParseBlockEnd(source);

        return result;
    }

    std::shared_ptr<InterpreterExpression> Parser::ParseList(Source &source) {
        assert(source.GetCurrentTokenType() == TokenType::LEFT_BRACKET);
        source.GetNextTokenType();

        auto result = make_shared<InterpreterLiteralList>();

        if (source.GetCurrentTokenType() != TokenType::RIGHT_BRACKET) {
            result->AddElement(ParseExpression(source));

            while (source.GetCurrentTokenType() == TokenType::COMMA) {
                source.GetNextTokenType();
                result->AddElement(ParseExpression(source));
            }
        }

        if (source.GetCurrentTokenType() != TokenType::RIGHT_BRACKET) {
            throw ParserError("']' expected", source.GetLineNumber());
        }

        source.GetNextTokenType();
        return result;
    }

}
