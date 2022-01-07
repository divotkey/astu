/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "VeloxAst.h"
#include "VeloxScanner.h"

// C++ Standard Library includes
#include <memory>

namespace astu {

	class ICharStream;
	class StringCharStream;

	class VeloxParser {
	public:

		VeloxParser();
		~VeloxParser();

		std::shared_ptr<VeloxNode> parse(std::shared_ptr<ICharStream> script);
		std::shared_ptr<VeloxNode> parseSingleStatement(const std::string & script);
		std::shared_ptr<VeloxNode> parseSingleStatement(std::shared_ptr<ICharStream> script);

	private:
		std::unique_ptr<VeloxScanner> m_scanner;
		std::shared_ptr<VeloxNode> m_root;


		std::shared_ptr<VeloxNode> parseStatement();
		std::shared_ptr<VeloxNode> parseStatementSequence(bool isBlock = true);
		std::shared_ptr<VeloxNode> parseBlock();
		std::shared_ptr<VeloxNode> parseFunctionDefinition();
		std::shared_ptr<VeloxNode> parseClassDefinition();
		std::shared_ptr<VeloxNode> parsePrint();
		std::shared_ptr<VeloxNode> parseImport();
		std::shared_ptr<VeloxNode> parseIdentStatement();
		std::shared_ptr<VeloxNode> parseGlobalStatement();
		std::shared_ptr<VeloxNode> parseWhile();
		std::shared_ptr<VeloxNode> parseFor();
		std::shared_ptr<VeloxNode> parseIf();
		std::shared_ptr<VeloxNode> parseReturn();
		std::shared_ptr<VeloxNode> parseName();
		std::shared_ptr<VeloxNode> parseSimpleName();
		std::shared_ptr<VeloxNode> parseAssignment(std::shared_ptr<VeloxNode> lhs);
		std::shared_ptr<VeloxNode> parseFunctionCall(std::shared_ptr<VeloxNode> lhs);
		std::shared_ptr<VeloxNode> parseAssignmentExpression();
		std::shared_ptr<VeloxNode> parseExpression();
		std::shared_ptr<VeloxNode> parseRelExpression();
		std::shared_ptr<VeloxNode> parseOrExpression();
		std::shared_ptr<VeloxNode> parseSimpleExpression();
		std::shared_ptr<VeloxNode> parseTerm();
		std::shared_ptr<VeloxNode> parseFactor();
		std::shared_ptr<VeloxNode> parseFactorIdent();
		std::shared_ptr<VeloxNode> parseFactorArray();
		std::shared_ptr<VeloxNode> parseObjectCreation();
		std::shared_ptr<VeloxNode> parseSelector(std::shared_ptr<VeloxNode> node);
		std::shared_ptr<StringCharStream> m_stringStream;
		bool isStatementStart();
		bool isFactor();
		bool isTerm();
		bool isRelation();

		VeloxNodeType infixOperatorToNodeType();
		VeloxNodeType relationOperatorToNodeType();
		VeloxNodeType assignmentNodeType();
		void readLeftParenthesis();
		void readRightParenthesis();
		void readBlockStart();
		void readBlockEnd();
		std::string readIdent();
		std::string readString();
		std::shared_ptr<VeloxNode> readSemicolon(std::shared_ptr<VeloxNode> result = nullptr, bool optional = false);
	};

} // end of namespace