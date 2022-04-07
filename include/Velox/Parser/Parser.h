// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "TokenType.h"
#include "Source.h"
#include "Velox/Interpreter/InterpreterStatementBlock.h"
#include "Velox/Interpreter/InterpreterExpression.h"
#include "Velox/Interpreter/Operators.h"

// C++ Standard Library includes
#include <memory>
#include <map>

namespace velox {

    class Parser {
    public:

        std::shared_ptr<InterpreterStatementBlock> Parse(Source &source);

    private:
        static const TokenType STATEMENT_START[];
        static const TokenType LVALUE_CHAIN[];
        static const TokenType REL_OP[];
        static const std::map<TokenType, ArithmeticOperator> OP_TO_TOK;
        static const std::map<TokenType, RelationalOperator> REL_OP_TO_TOK;
        static const std::map<TokenType, LogicalOperator> LOG_OP_TO_TOK;

        std::shared_ptr<InterpreterStatementBlock> ParseStatementBlock(Source &source);
        std::shared_ptr<InterpreterStatement> ParseStatement(Source &source);
        std::shared_ptr<InterpreterStatement> ParseIdentStatement(Source &source);
        std::shared_ptr<InterpreterStatement> ParseAssignment(Source &source, std::shared_ptr<InterpreterExpression> lValue);

        std::shared_ptr<InterpreterStatement>
        ParseAssignmentOperator(Source &source, std::shared_ptr<InterpreterExpression> lValue, ArithmeticOperator op);
        std::shared_ptr<InterpreterStatement> ParseFunctionDefinition(Source &source);
        std::shared_ptr<InterpreterStatement> ParseReturnStatement(Source &source);
        std::shared_ptr<InterpreterStatement> ParseIfStatement(Source &source);
        std::shared_ptr<InterpreterStatement> ParseWhileStatement(Source &source);

        std::shared_ptr<InterpreterExpression> ParseVariable(Source &source, bool location);
        std::shared_ptr<InterpreterExpression> ParseExpression(Source &source);
        std::shared_ptr<InterpreterExpression> ParseMemberAccess(Source &source, std::shared_ptr<InterpreterExpression> lValue);
        std::shared_ptr<InterpreterExpression> ParseSimpleName(Source &source);
        std::shared_ptr<InterpreterExpression> ParseOrExpression(Source &source);
        std::shared_ptr<InterpreterExpression> ParseRelExpression(Source &source);
        std::shared_ptr<InterpreterExpression> ParseSimpleExpression(Source &source);
        std::shared_ptr<InterpreterExpression> ParseTerm(Source &source);
        std::shared_ptr<InterpreterExpression> ParseFactor(Source &source);
        std::shared_ptr<InterpreterExpression> ParseFactorIdent(Source &source);
        std::shared_ptr<InterpreterExpression> ParseFunctionCall(Source &source, std::shared_ptr<InterpreterExpression> function);

        void ParseSemicolon(Source &source);
        void ParseRightParenthesis(Source &source);
        void ParseLeftParenthesis(Source &source);
        void ParseBlockStart(Source &source);
        void ParseBlockEnd(Source &source);

    };

}



