/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Parser/ParserError.h"
#include "Velox/Parser/TokenType.h"
#include "Velox/Parser/ISource.h"
#include "Velox/Interpreter/InterpreterStatementBlock.h"
#include "Velox/Interpreter/InterpreterScript.h"
#include "Velox/Interpreter/InterpreterFunctionDefinition.h"
#include "Velox/Interpreter/InterpreterConstructorCall.h"
#include "Velox/Interpreter/InterpreterExpression.h"
#include "Velox/Interpreter/InterpreterFunctionCall.h"
#include "Velox/Interpreter/Operators.h"

// C++ Standard Library includes
#include <memory>
#include <map>

namespace velox {

    // Forward declaration
    class InterpreterInstantDefinition;

    class Parser {
    public:

        std::unique_ptr<InterpreterScript> Parse(ISource &source);

        /**
         * Parses a single instant definition.
         *
         * @param source    the source that contains the instant definition
         * @return the newly created instant definition
         * @throws ParserError in case of an error
         */
        std::shared_ptr<InterpreterInstantDefinition> ParseInstantDefinition(ISource &source);

    private:
        static const TokenType STATEMENT_START[];
        static const TokenType LVALUE_CHAIN[];
        static const TokenType REL_OP[];
        static const std::map<TokenType, ArithmeticOperator> OP_TO_TOK;
        static const std::map<TokenType, RelationalOperator> REL_OP_TO_TOK;
        static const std::map<TokenType, LogicalOperator> LOG_OP_TO_TOK;

        std::shared_ptr<InterpreterStatementBlock> ParseStatementBlock(ISource &source, bool loopBody);
        std::shared_ptr<InterpreterStatement> ParseStatementOrBlock(ISource &source, bool loopBody);

        std::shared_ptr<InterpreterStatement> ParseStatement(ISource &source);
        std::shared_ptr<InterpreterStatement> ParseIncludeStatement(ISource &source);
        std::shared_ptr<InterpreterStatement> ParseForHeaderStatement(ISource &source);

        std::shared_ptr<InterpreterStatement> ParseAssignment(ISource &source, std::shared_ptr<InterpreterExpression> lValue);
        std::shared_ptr<InterpreterStatement> ParseOptionalAssignment(ISource &source, std::shared_ptr<InterpreterExpression> lValue);

        std::shared_ptr<InterpreterStatement>
        ParseAssignmentOperator(ISource &source, std::shared_ptr<InterpreterExpression> lValue, ArithmeticOperator op);
        std::shared_ptr<InterpreterFunctionDefinition> ParseFunctionDefinition(ISource &source);
        std::shared_ptr<InterpreterStatement> ParseReturnStatement(ISource &source);
        std::shared_ptr<InterpreterStatement> ParseGlobalStatement(ISource &source);
        std::shared_ptr<InterpreterStatement> ParseIfStatement(ISource &source);
        std::shared_ptr<InterpreterStatement> ParseWhileStatement(ISource &source);
        std::shared_ptr<InterpreterStatement> ParseDoWhileStatement(ISource &source);
        std::shared_ptr<InterpreterStatement> ParseLoopStatement(ISource &source);
        std::shared_ptr<InterpreterStatement> ParseForStatement(ISource &source);
        std::shared_ptr<InterpreterStatement> ParseClassDefinition(ISource &source);
        std::shared_ptr<InterpreterExpression> ParseInstantRealization(ISource &source);
        std::shared_ptr<InterpreterExpression> ParseNewStatement(ISource &source);
        std::shared_ptr<InterpreterExpression> ParseOptionalSelector(ISource &source, std::shared_ptr<InterpreterExpression> lValue);
        std::shared_ptr<InterpreterExpression> ParseExpression(ISource &source);
        std::shared_ptr<InterpreterExpression> ParseMemberAccess(ISource &source, std::shared_ptr<InterpreterExpression> lValue);
        std::shared_ptr<InterpreterExpression> ParseListAccess(ISource &source, std::shared_ptr<InterpreterExpression> lValue);
        std::shared_ptr<InterpreterExpressionSimpleName> ParseSimpleName(ISource &source);
        std::shared_ptr<InterpreterExpression> ParseOrExpression(ISource &source);
        std::shared_ptr<InterpreterExpression> ParseRelExpression(ISource &source);
        std::shared_ptr<InterpreterExpression> ParseSimpleExpression(ISource &source);
        std::shared_ptr<InterpreterExpression> ParseTerm(ISource &source);
        std::shared_ptr<InterpreterExpression> ParseFactor(ISource &source);
        std::shared_ptr<InterpreterExpression> ParseFactorIdent(ISource &source);
        std::shared_ptr<InterpreterExpression> ParseColor(ISource &source);
        std::shared_ptr<InterpreterExpression> ParseVector(ISource &source);
        std::shared_ptr<InterpreterExpression> ParseList(ISource &source);

        std::shared_ptr<InterpreterFunctionCall>
        ParseFunctionCall(ISource &source, std::shared_ptr<InterpreterExpression> function);
        std::shared_ptr<InterpreterConstructorCall> ParseConstructorCall(ISource &source);

        void ParseSemicolon(ISource &source);
        void ParseOptionalSemicolon(ISource &source);
        void ParseComma(ISource &source);
        void ParseRightParenthesis(ISource &source);
        void ParseLeftParenthesis(ISource &source);
        void ParseBlockStart(ISource &source);
        void ParseBlockEnd(ISource &source);
        double ParseReal(ISource &source);

    };

}



