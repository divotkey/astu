/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Velox/VeloxParser.h"
#include "Velox/VeloxAst.h"
#include "Velox/VeloxScanner.h"
#include "Velox/StringCharStream.h"

// C++ Standard Library includes
#include <cassert>

namespace astu {

	VeloxParser::VeloxParser()
		: m_scanner(std::make_unique<VeloxScanner>())
	{
		// Intentionally left empty.
	}

	VeloxParser::~VeloxParser()
	{
		// Intentionally left empty.
	}

	std::shared_ptr<VeloxNode> VeloxParser::parse(std::shared_ptr<ICharStream> script)
	{
		try {
			m_scanner->reset(script);
			m_scanner->nextToken();
			return parseStatementSequence(false);
		}
		catch (...) {
			m_scanner->reset(nullptr);
			throw;
		}
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseSingleStatement(std::shared_ptr<ICharStream> script)
	{
		try {
			m_scanner->reset(script);
			m_scanner->nextToken();
			return parseStatement();
		}
		catch (...) {
			m_scanner->reset(nullptr);
			throw;
		}
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseSingleStatement(const std::string & script)
	{
		if (!m_stringStream) {
			m_stringStream = std::make_shared<StringCharStream>(script);
		}
		else {
			m_stringStream->reset(script);
		}
		return parseSingleStatement(m_stringStream);
	}

	std::shared_ptr<VeloxNode> VeloxParser::parsePrint()
	{
		assert(m_scanner->getToken() == VeloxScanner::PRINT);
		m_scanner->nextToken();

		auto result = VeloxNode::create(VeloxNodeType::PRINT);
		result->setPosition(m_scanner->getPrevTokenPosition());
		result->addChild(parseExpression());
		return result;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseImport()
	{
		assert(m_scanner->getToken() == VeloxScanner::IMPORT);
		m_scanner->nextToken();

		auto result = VeloxNode::create(VeloxNodeType::IMPORT);
		result->setPosition(m_scanner->getPrevTokenPosition());
		result->setStringData(readString());
		return result;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseIdentStatement()
	{
		assert(m_scanner->getToken() == VeloxScanner::IDENT);
		auto node = parseName();

		switch (m_scanner->getToken()) {
		case VeloxScanner::ASSIGNMENT:
		case VeloxScanner::ASSIGN_ADD:
		case VeloxScanner::ASSIGN_SUB:
		case VeloxScanner::ASSIGN_MUL:
		case VeloxScanner::ASSIGN_DIV:
		case VeloxScanner::ASSIGN_MOD:
			return parseAssignment(node);

		case VeloxScanner::INCREMENT: {
			auto result = VeloxNode::create(VeloxNodeType::INCREMENT);
			result->addChild(node);
			result->setPosition(m_scanner->getTokenPosition());
			m_scanner->nextToken();
			return result;
		}

		case VeloxScanner::DECREMENT:
		{
			auto result = VeloxNode::create(VeloxNodeType::DECREMENT);
			result->addChild(node);
			result->setPosition(m_scanner->getTokenPosition());
			m_scanner->nextToken();
			return result;
		}

		default:
			// ignore
			return node;
		}
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseGlobalStatement()
	{
		assert(m_scanner->getToken() == VeloxScanner::GLOBAL);
		m_scanner->nextToken();
		auto node = VeloxNode::create(VeloxNodeType::GLOBAL);
		node->addChild(parseSimpleName());

		switch (m_scanner->getToken()) {
		case VeloxScanner::ASSIGNMENT:
		case VeloxScanner::ASSIGN_ADD:
		case VeloxScanner::ASSIGN_SUB:
		case VeloxScanner::ASSIGN_MUL:
		case VeloxScanner::ASSIGN_DIV:
		case VeloxScanner::ASSIGN_MOD:
			return parseAssignment(node);

		default:
			// ignore
			return node;
		}
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseWhile()
	{
		assert(m_scanner->getToken() == VeloxScanner::WHILE);
		m_scanner->nextToken();
		readLeftParenthesis();
		auto result = VeloxNode::create(VeloxNodeType::WHILE);
		result->addChild(parseExpression());
		readRightParenthesis();
		result->addChild(parseStatement());
		return result;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseFor()
	{
		assert(m_scanner->getToken() == VeloxScanner::FOR);
		m_scanner->nextToken();
		readLeftParenthesis();
		auto result = VeloxNode::create(VeloxNodeType::FOR);

		auto initializer = VeloxNode::create(VeloxNodeType::FOR_INITIALIZER);
		if (m_scanner->getToken() != VeloxScanner::SEMICOLON) {
			initializer->addChild(parseAssignmentExpression());
			while (m_scanner->getToken() == VeloxScanner::COMMA) {
				m_scanner->nextToken();
				initializer->addChild(parseAssignmentExpression());
			}
		}
		result->addChild(initializer);
		readSemicolon();

		result->addChild(parseExpression());
		readSemicolon();

		auto updater = VeloxNode::create(VeloxNodeType::FOR_UPDATER);
		if (m_scanner->getToken() != VeloxScanner::RIGHT_PARENTHESIS) {
			updater->addChild(parseAssignmentExpression());
			while (m_scanner->getToken() == VeloxScanner::COMMA) {
				m_scanner->nextToken();
				updater->addChild(parseExpression());
			}
		}
		result->addChild(updater);

		readRightParenthesis();
		result->addChild(parseStatement());
		return result;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseIf()
	{
		assert(m_scanner->getToken() == VeloxScanner::IF);
		m_scanner->nextToken();
		readLeftParenthesis();
		auto result = VeloxNode::create(VeloxNodeType::IF);
		result->addChild(parseExpression());
		readRightParenthesis();
		result->addChild(parseStatement());
		if (m_scanner->getToken() == VeloxScanner::ELSE) {
			m_scanner->nextToken();
			result->addChild(parseStatement());
		}
		return result;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseReturn()
	{
		assert(m_scanner->getToken() == VeloxScanner::RETURN);
		m_scanner->nextToken();

		auto result = VeloxNode::create(VeloxNodeType::RETURN);
		if (m_scanner->getToken() != VeloxScanner::SEMICOLON) {
			result->addChild(parseExpression());
		}
		return result;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseStatement()
	{
		if (!isStatementStart()) {
			throw VeloxCompilerException("statement expected", m_scanner->getLine());
		}

		switch (m_scanner->getToken()) {
		case VeloxScanner::FUNCTION:
			return parseFunctionDefinition();
		case VeloxScanner::CLASS:
			return parseClassDefinition();
		case VeloxScanner::INCREMENT:
		case VeloxScanner::DECREMENT:
			return readSemicolon(parseAssignmentExpression());
		case VeloxScanner::NEW:
			return readSemicolon(parseExpression());
		case VeloxScanner::IDENT:
			return readSemicolon(parseIdentStatement());
		case VeloxScanner::GLOBAL:
			return readSemicolon(parseGlobalStatement());
		case VeloxScanner::IF:
			return parseIf();
		case VeloxScanner::WHILE:
			return parseWhile();
		case VeloxScanner::FOR:
			return parseFor();
		case VeloxScanner::PRINT:
			return readSemicolon(parsePrint());
		case VeloxScanner::RETURN:
			return readSemicolon(parseReturn());
		case VeloxScanner::BLOCK_START:
			return parseBlock();
		case VeloxScanner::IMPORT:
			return readSemicolon(parseImport(), true);
		default:
			throw VeloxCompilerException("Syntax error", m_scanner->getLine());
		}
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseStatementSequence(bool isBlock)
	{
		auto block = VeloxNode::create(isBlock ? VeloxNodeType::BLOCK : VeloxNodeType::SEQUENCE);
		while (isStatementStart()) {
			block->addChild(parseStatement());
		}

		return block;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseBlock()
	{
		readBlockStart();
		auto result = parseStatementSequence();
		readBlockEnd();
		return result;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseFunctionDefinition()
	{
		if (m_scanner->getToken() != VeloxScanner::FUNCTION) {
			throw VeloxCompilerException("function definition expected", m_scanner->getLine());
		}
		m_scanner->nextToken();
		auto result = VeloxNode::create(VeloxNodeType::FUNCTION_DECLARATION);
		result->setStringData(readIdent());
		readLeftParenthesis();
		auto params = VeloxNode::create(VeloxNodeType::FORMAL_PARAMETER);
		if (m_scanner->getToken() == VeloxScanner::IDENT) {
			params->addChild(parseSimpleName());
			while (m_scanner->getToken() == VeloxScanner::COMMA) {
				m_scanner->nextToken();
				params->addChild(parseSimpleName());
			}
		}
		readRightParenthesis();
		result->addChild(params);
		result->addChild(parseBlock());
		return result;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseClassDefinition()
	{
		assert(m_scanner->getToken() == VeloxScanner::CLASS);
		m_scanner->nextToken();

		auto result = VeloxNode::create(VeloxNodeType::CLASS_DECLARATION);
		result->setStringData(readIdent());
		readBlockStart();
		while (m_scanner->getToken() != VeloxScanner::BLOCK_END) {
			result->addChild(parseFunctionDefinition());
		}
		readBlockEnd();
		return result;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseName()
	{
		if (m_scanner->getToken() != VeloxScanner::IDENT) {
			throw VeloxCompilerException("identifier expected", m_scanner->getLine());
		}

		return parseSelector(parseSimpleName());
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseSimpleName()
	{
		auto node = VeloxNode::create(VeloxNodeType::SIMPLE_NAME);
		node->setStringData(readIdent());
		node->setPosition(m_scanner->getPrevTokenPosition());

		return node;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseAssignment(std::shared_ptr<VeloxNode> lhs)
	{
		auto result = VeloxNode::create(assignmentNodeType());
		result->setPosition(m_scanner->getTokenPosition());
		m_scanner->nextToken();
		result->addChild(lhs);
		result->addChild(parseAssignmentExpression());
		return result;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseFunctionCall(std::shared_ptr<VeloxNode> node)
	{
		auto result = VeloxNode::create(VeloxNodeType::FUNCTION_CALL);
		result->setPosition(m_scanner->getPrevTokenPosition());
		result->addChild(node);

		readLeftParenthesis();
		auto params = VeloxNode::create(VeloxNodeType::FUNCTION_PARAMS);
		if (m_scanner->getToken() != VeloxScanner::RIGHT_PARENTHESIS) {
			params->addChild(parseAssignmentExpression());
			while (m_scanner->getToken() == VeloxScanner::COMMA) {
				m_scanner->nextToken();
				params->addChild(parseAssignmentExpression());
			}
		}

		result->addChild(params);
		readRightParenthesis();
		return result;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseAssignmentExpression()
	{
		auto exp = parseExpression();
		switch (m_scanner->getToken()) {
			case VeloxScanner::ASSIGNMENT: 
			case VeloxScanner::ASSIGN_ADD: 
			case VeloxScanner::ASSIGN_SUB:
			case VeloxScanner::ASSIGN_MUL:
			case VeloxScanner::ASSIGN_DIV:
			case VeloxScanner::ASSIGN_MOD:
			{
				auto tmp = VeloxNode::create(assignmentNodeType());
				m_scanner->nextToken();
				tmp->addChild(exp);
				tmp->addChild(parseAssignmentExpression());
				exp = tmp;
				break;
			}
		}

		return exp;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseExpression()
	{
		auto exp = parseOrExpression();
		while (m_scanner->getToken() == VeloxScanner::LOG_AND) {
			auto lhs = exp;
			exp = VeloxNode::create(VeloxNodeType::LOG_AND);
			m_scanner->nextToken();
			exp->addChild(lhs);
			exp->addChild(parseOrExpression());
		}

		return exp;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseRelExpression()
	{
		auto expr = parseSimpleExpression();
		while (isRelation()) {
			auto lhs = expr;
			expr = VeloxNode::create(relationOperatorToNodeType());
			expr->setPosition(m_scanner->getTokenPosition());
			m_scanner->nextToken();
			expr->addChild(lhs);
			expr->addChild(parseSimpleExpression());
		}

		return expr;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseOrExpression()
	{
		auto exp = parseRelExpression();
		while (m_scanner->getToken() == VeloxScanner::LOG_OR) {
			auto lhs = exp;
			exp = VeloxNode::create(VeloxNodeType::LOG_OR);
			m_scanner->nextToken();
			exp->addChild(lhs);
			exp->addChild(parseRelExpression());
		}

		return exp;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseSimpleExpression()
	{
		auto term = parseTerm();
		while (isTerm()) {
			auto lhs = term;
			term = VeloxNode::create(infixOperatorToNodeType());
			term->setPosition(m_scanner->getTokenPosition());
			m_scanner->nextToken();
			term->addChild(lhs);
			term->addChild(parseTerm());
		}

		return term;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseTerm()
	{
		auto factor = parseFactor();
		while (isFactor()) {
			auto lhs = factor;
			factor = VeloxNode::create(infixOperatorToNodeType());
			factor->setPosition(m_scanner->getTokenPosition());
			m_scanner->nextToken();
			factor->addChild(lhs);
			factor->addChild(parseFactor());
		}

		return factor;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseFactor()
	{
		switch (m_scanner->getToken()) {
		case VeloxScanner::SUB: {
			auto result = VeloxNode::create(VeloxNodeType::UNARY_MINUS);
			m_scanner->nextToken();
			result->addChild(parseFactor());
			return result;
		}

		case VeloxScanner::ADD: {
			auto result = VeloxNode::create(VeloxNodeType::UNARY_PLUS);
			m_scanner->nextToken();
			result->addChild(parseFactor());
			return result;
		}

		case VeloxScanner::NOT: {
			auto result = VeloxNode::create(VeloxNodeType::NOT);
			m_scanner->nextToken();
			result->addChild(parseFactor());
			return result;
		}

		case VeloxScanner::NEW:
			return parseSelector(parseObjectCreation());

		case VeloxScanner::IDENT:
			return parseFactorIdent();

		case VeloxScanner::STRING: {
			auto result = VeloxNode::create(VeloxNodeType::STRING_LITERAL);
			result->setStringData(m_scanner->getString());
			m_scanner->nextToken();
			return result;
		}

		case VeloxScanner::INCREMENT: {
			auto result = VeloxNode::create(VeloxNodeType::INCREMENT);
			m_scanner->nextToken();
			result->addChild(parseFactorIdent());
			return result;
		}

		case VeloxScanner::DECREMENT: {
			auto result = VeloxNode::create(VeloxNodeType::DECREMENT);
			m_scanner->nextToken();
			result->addChild(parseFactorIdent());
			return result;
		}

		case VeloxScanner::INT: {
			auto result = VeloxNode::create(VeloxNodeType::INTEGER_LITERAL);
			result->setIntegerData(m_scanner->getIntValue());
			m_scanner->nextToken();
			return result;
		}

		case VeloxScanner::REAL: {
			auto result = VeloxNode::create(VeloxNodeType::REAL_LITERAL);
			result->setRealData(m_scanner->getDoubleValue());
			m_scanner->nextToken();
			return result;
		}

		case VeloxScanner::TRUE: {
			auto result = VeloxNode::create(VeloxNodeType::BOOL_LITERAL);
			result->setIntegerData(1);
			m_scanner->nextToken();
			return result;
		}

		case VeloxScanner::FALSE: {
			auto result = VeloxNode::create(VeloxNodeType::BOOL_LITERAL);
			result->setIntegerData(0);
			m_scanner->nextToken();
			return result;
		}

		case VeloxScanner::LEFT_BRACKET:
			return parseFactorArray();

		case VeloxScanner::LEFT_PARENTHESIS: {
			m_scanner->nextToken();
			auto result = parseExpression();
			readRightParenthesis();
			return result;
		}

		default:
			throw VeloxCompilerException("Syntax error", m_scanner->getLine());
		}
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseFactorIdent()
	{
		auto result = parseSelector(parseSimpleName());

		// postfix operation
		if (m_scanner->getToken() == VeloxScanner::INCREMENT) {
			m_scanner->nextToken();
			auto postFix = VeloxNode::create(VeloxNodeType::POST_INCREMENT);
			postFix->addChild(result);
			return postFix;
		} else if (m_scanner->getToken() == VeloxScanner::DECREMENT) {
			m_scanner->nextToken();
			auto postFix = VeloxNode::create(VeloxNodeType::POST_DECREMENT);
			postFix->addChild(result);
			return postFix;
		}

		return result;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseFactorArray()
	{
		assert(m_scanner->getToken() == VeloxScanner::LEFT_BRACKET);
		m_scanner->nextToken();

		auto result = VeloxNode::create(VeloxNodeType::ARRAY_INITIALIZER);

		if (m_scanner->getToken() != VeloxScanner::RIGHT_BRACKET) {
			result->addChild(parseExpression());
			while (m_scanner->getToken() == VeloxScanner::COMMA) {
				m_scanner->nextToken();
				result->addChild(parseExpression());
			}
		}

		if (m_scanner->getToken() != VeloxScanner::RIGHT_BRACKET) {
			throw VeloxCompilerException("']' expected", m_scanner->getLine());
		}
		m_scanner->nextToken();

		return result;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseObjectCreation()
	{
		assert(m_scanner->getToken() == VeloxScanner::NEW);
		m_scanner->nextToken();
		auto result = VeloxNode::create(VeloxNodeType::OBJECT_CREATION);
		result->setPosition(m_scanner->getPrevTokenPosition());
		result->setStringData(readIdent());
		readLeftParenthesis();
		auto params = VeloxNode::create(VeloxNodeType::FUNCTION_PARAMS);

		if (m_scanner->getToken() != VeloxScanner::RIGHT_PARENTHESIS) {
			params->addChild(parseExpression());
			while (m_scanner->getToken() == VeloxScanner::COMMA) {
				m_scanner->nextToken();
				params->addChild(parseExpression());
			}
		}
		result->addChild(params);
		readRightParenthesis();

		return result;
	}

	std::shared_ptr<VeloxNode> VeloxParser::parseSelector(std::shared_ptr<VeloxNode> node)
	{
		if (m_scanner->getToken() == VeloxScanner::LEFT_BRACKET) {
			m_scanner->nextToken();
			auto result = VeloxNode::create(VeloxNodeType::ARRAY_ACCESS);
			result->setPosition(m_scanner->getPrevTokenPosition());
			result->addChild(node);
			result->addChild(parseExpression());
			if (m_scanner->getToken() != VeloxScanner::RIGHT_BRACKET) {
				throw VeloxCompilerException("Syntax error: ']' expected", m_scanner->getLine());
			}
			m_scanner->nextToken();
			return parseSelector(result);
		}
		if (m_scanner->getToken() == VeloxScanner::DOT) {
			m_scanner->nextToken();
			auto result = VeloxNode::create(VeloxNodeType::FIELD_ACCESS);
			result->setStringData(readIdent());
			result->setPosition(m_scanner->getPrevTokenPosition());
			result->addChild(node);
			return parseSelector(result);
		}
		else if (m_scanner->getToken() == VeloxScanner::LEFT_PARENTHESIS) {
			return parseSelector(parseFunctionCall(node));
		}

		return node;
	}

	bool VeloxParser::isStatementStart()
	{
		switch (m_scanner->getToken()) {
		case VeloxScanner::FUNCTION:
		case VeloxScanner::CLASS:
		case VeloxScanner::IDENT:
		case VeloxScanner::PRINT:
		case VeloxScanner::WHILE:
		case VeloxScanner::FOR:
		case VeloxScanner::BLOCK_START:
		case VeloxScanner::IF:
		case VeloxScanner::INCREMENT:
		case VeloxScanner::DECREMENT:
		case VeloxScanner::RETURN:
		case VeloxScanner::GLOBAL:
		case VeloxScanner::NEW:
		case VeloxScanner::IMPORT:
			return true;

		default:
			return false;
		}
	}

	bool VeloxParser::isFactor()
	{
		switch (m_scanner->getToken()) {
		case VeloxScanner::MUL:
		case VeloxScanner::DIV:
		case VeloxScanner::MOD:
			return true;

		default:
			return false;
		}
	}

	bool VeloxParser::isTerm()
	{
		switch (m_scanner->getToken()) {
		case VeloxScanner::ADD:
		case VeloxScanner::SUB:
		case VeloxScanner::MOD:
			return true;

		default:
			return false;
		}
	}

	bool VeloxParser::isRelation()
	{
		switch (m_scanner->getToken()) {
		case VeloxScanner::LESS_THAN:
		case VeloxScanner::LESS_EQUAL:
		case VeloxScanner::GREATER_THAN:
		case VeloxScanner::GREATER_EQUAL:
		case VeloxScanner::EQUAL:
		case VeloxScanner::NOT_EQUAL:
			return true;

		default:
			return false;
		}
	}

	VeloxNodeType VeloxParser::infixOperatorToNodeType()
	{
		switch (m_scanner->getToken()) {
		case VeloxScanner::ADD:
			return VeloxNodeType::ADDITION;

		case VeloxScanner::SUB:
			return VeloxNodeType::SUBTRACTION;

		case VeloxScanner::MOD:
			return VeloxNodeType::MODULO;

		case VeloxScanner::MUL:
			return VeloxNodeType::MULTIPLICATION;

		case VeloxScanner::DIV:
			return VeloxNodeType::DIVISION;

		case VeloxScanner::LOG_AND:
			return VeloxNodeType::LOG_AND;

		case VeloxScanner::LOG_OR:
			return VeloxNodeType::LOG_OR;

		default:
			throw VeloxCompilerException("Internal compiler error", m_scanner->getLine());
		}
	}

	VeloxNodeType VeloxParser::relationOperatorToNodeType()
	{
		switch (m_scanner->getToken()) {
		case VeloxScanner::EQUAL:
			return VeloxNodeType::EQUAL;

		case VeloxScanner::NOT_EQUAL:
			return VeloxNodeType::NOT_EQUAL;

		case VeloxScanner::LESS_THAN:
			return VeloxNodeType::LESS_THAN;

		case VeloxScanner::LESS_EQUAL:
			return VeloxNodeType::LESS_EQUAL;

		case VeloxScanner::GREATER_THAN:
			return VeloxNodeType::GREATER_THAN;

		case VeloxScanner::GREATER_EQUAL:
			return VeloxNodeType::GREATER_EQUAL;

		default:
			throw VeloxCompilerException("Internal compiler error", m_scanner->getLine());
		}
	}

	VeloxNodeType VeloxParser::assignmentNodeType()
	{
		switch (m_scanner->getToken()) {
		case VeloxScanner::ASSIGNMENT:
			return VeloxNodeType::ASSIGNMENT;

		case VeloxScanner::ASSIGN_ADD:
			return VeloxNodeType::ASSIGN_ADD;

		case VeloxScanner::ASSIGN_SUB:
			return VeloxNodeType::ASSIGN_SUB;

		case VeloxScanner::ASSIGN_MUL:
			return VeloxNodeType::ASSIGN_MUL;

		case VeloxScanner::ASSIGN_DIV:
			return VeloxNodeType::ASSIGN_DIV;

		case VeloxScanner::ASSIGN_MOD:
			return VeloxNodeType::ASSIGN_MOD;

		default:
			throw VeloxCompilerException("Internal compiler error", m_scanner->getLine());
		}
	}

	void VeloxParser::readLeftParenthesis()
	{
		if (m_scanner->getToken() != VeloxScanner::LEFT_PARENTHESIS) {
			throw VeloxCompilerException("syntax error: '(' expected", m_scanner->getLine());
		}
		m_scanner->nextToken();
	}

	void VeloxParser::readRightParenthesis()
	{
		if (m_scanner->getToken() != VeloxScanner::RIGHT_PARENTHESIS) {
			throw VeloxCompilerException("syntax error: ')' expected", m_scanner->getLine());
		}
		m_scanner->nextToken();
	}

	void VeloxParser::readBlockStart()
	{
		if (m_scanner->getToken() != VeloxScanner::BLOCK_START) {
			throw VeloxCompilerException("'{' expected", m_scanner->getLine());
		}
		m_scanner->nextToken();
	}

	void VeloxParser::readBlockEnd()
	{
		if (m_scanner->getToken() != VeloxScanner::BLOCK_END) {
			throw VeloxCompilerException("'}' expected", m_scanner->getLine());
		}
		m_scanner->nextToken();
	}

	std::string VeloxParser::readIdent()
	{
		if (m_scanner->getToken() != VeloxScanner::IDENT) {
			throw VeloxCompilerException("identifier expected", m_scanner->getLine());
		}
		auto result = m_scanner->getIdent();
		m_scanner->nextToken();
		return result;
	}

	std::string VeloxParser::readString()
	{
		if (m_scanner->getToken() != VeloxScanner::STRING) {
			throw VeloxCompilerException("string expected", m_scanner->getLine());
		}
		auto result = m_scanner->getString();
		m_scanner->nextToken();
		return result;
	}

	std::shared_ptr<VeloxNode> VeloxParser::readSemicolon(std::shared_ptr<VeloxNode> result, bool optional)
	{
		if (m_scanner->getToken() != VeloxScanner::SEMICOLON) {
			if (!optional) {
				throw VeloxCompilerException("syntax error: ';' expected", m_scanner->getLine());
			}
		}
		else {
			m_scanner->nextToken();
		}
		return result;
	}

} // end of namespace