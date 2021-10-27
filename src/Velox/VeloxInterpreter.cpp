/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#define _USE_MATH_DEFINES

// Local includes
#include "Velox/VeloxInterpreter.h"
#include "Velox/VeloxAst.h"
#include "Velox/StandardVeloxHooks.h"
#include "Velox/VeloxScope.h"
#include "Velox/VeloxItem.h"
#include "Velox/VeloxHook.h"
#include "Velox/VeloxScript.h"
#include "Velox/IVeloxImporter.h"
#include "Util/StringUtils.h"
#include "Velox/VeloxInterpreterException.h"
#include "Velox/StringCharStream.h"
#include "Velox/VeloxParser.h"

// C++ Standard Library includes
#include <cmath>
#include <limits>
#include <cassert>

#if NDEBUG
#define RETURN_VERIFIED(a) return a;
#else
#define RETURN_VERIFIED(a) {size_t lvl = m_scope->getLevel(); auto && result = a; assert(lvl == m_scope->getLevel()); return result;}
#endif

using namespace std;

namespace astu {

	vector<unique_ptr<VeloxInterpreter>> VeloxInterpreter::pool;

	std::unique_ptr<VeloxInterpreter> VeloxInterpreter::Obtain(shared_ptr<VeloxNode> root, const std::string& sourcePath)
	{
		unique_ptr<VeloxInterpreter> result;
		if (pool.empty()) {
			result = make_unique<VeloxInterpreter>();
		} else {
			result = move(pool.back());
			pool.pop_back();
		}

		result->Initialize(root, sourcePath);
		return result;
	}

	void VeloxInterpreter::Free(std::unique_ptr<VeloxInterpreter> interpreter)
	{
		interpreter->reset();
		pool.push_back(move(interpreter));
	}


	VeloxInterpreter::VeloxInterpreter()
	{
		addHook(std::make_shared<SqrtVeloxHook>());
		addHook(std::make_shared<CosVeloxHook>());
		addHook(std::make_shared<SinVeloxHook>());
		addHook(std::make_shared<TanVeloxHook>());
		addHook(std::make_shared<AtanVeloxHook>());
		addHook(std::make_shared<Atan2VeloxHook>());
		addHook(std::make_shared<FloorVeloxHook>());
		addHook(std::make_shared<ToDegreesVeloxHook>());
		addHook(std::make_shared<ToRadiansVeloxHook>());
		addHook(std::make_shared<CeilVeloxHook>());
		addHook(std::make_shared<PowVeloxHook>());
		addHook(std::make_shared<RealVeloxHook>());
		addHook(std::make_shared<IntVeloxHook>());

		VeloxParser parser;
		m_functions["max"] = parser.parseSingleStatement("function max(a, b) { if (a > b) return a; return b; }");
		m_functions["min"] = parser.parseSingleStatement("function min(a, b) { if (a < b) return a; return b; }");
		m_functions["abs"] = parser.parseSingleStatement("function abs(a) { if (a < 0) return -a; return a;}");
		reset();
	}

	VeloxInterpreter::~VeloxInterpreter()
	{
		// Intentionally left empty.
	}

	void VeloxInterpreter::reset()
	{
		m_rootNode.reset();
	}

	void VeloxInterpreter::execute()
	{
		interpretStatement(*m_rootNode);
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluate(const VeloxItem & function)
	{
		auto params = VeloxNode::create(VeloxNodeType::FUNCTION_PARAMS);
		return evaluateScriptFunctionCall(function, *params);
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluate(const VeloxItem & function, const FunctionParameters & params)
	{
		// look for item parameters
		bool scopeUsed = false;
		size_t itemCnt = 0;
		for (const auto & param : params.getParamNode()) {
			if (param->getType() == VeloxNodeType::SIMPLE_NAME) {
				if (!scopeUsed) {
					m_scope = VeloxScope::create(std::move(m_scope));
					scopeUsed = true;
				}
				m_scope->putItem(param->getStringData(), params.getItems()[itemCnt++]);
			}
		}

		auto result = evaluateScriptFunctionCall(function, params.getParamNode());

		if (scopeUsed) {
			m_scope = m_scope->detachFromParent();
		}
		return result;
	}

	void VeloxInterpreter::setPrintFunc(std::function<void(const std::string&)> func)
	{
		m_printFunc = func;
	}

	void VeloxInterpreter::setWarnFunc(std::function<void(const std::string&)> func)
	{
		m_warnFunc = func;
	}

	void VeloxInterpreter::Initialize(std::shared_ptr<VeloxNode> root, const std::string& sourcePath)
	{
		assert(root);
		m_rootNode = root;
		m_sourcePath = sourcePath;
		openScope();
		addHooks(*m_scope);
		m_scope->putItem("PI", VeloxItem::createRealValue(M_PI));
		m_scope->putItem("E", VeloxItem::createRealValue(M_E));
		m_scope->putItem("MAX_INT", VeloxItem::createIntValue(std::numeric_limits<int>::max()));
		m_scope->putItem("MIN_INT", VeloxItem::createIntValue(std::numeric_limits<int>::min()));
		m_scope->putItem("NULL", VeloxItem::createUndefined());
		for (auto & it : m_functions) {
			m_scope->putItem(it.first, VeloxItem::createFunction(it.second));
		}

		functionLookahead(*m_rootNode, false);
	}

	// void VeloxInterpreter::setScript(std::shared_ptr<VeloxScript> script)
	// {
	// 	m_script = script;
	// 	m_scope = nullptr;
	// 	openScope();
	// 	addHooks(*m_scope);
	// 	m_scope->putItem("PI", VeloxItem::createRealValue(M_PI));
	// 	m_scope->putItem("E", VeloxItem::createRealValue(M_E));
	// 	m_scope->putItem("MAX_INT", VeloxItem::createIntValue(std::numeric_limits<int>::max()));
	// 	m_scope->putItem("MIN_INT", VeloxItem::createIntValue(std::numeric_limits<int>::min()));
	// 	m_scope->putItem("NULL", VeloxItem::createUndefined());
	// 	for (auto & it : m_functions) {
	// 		m_scope->putItem(it.first, VeloxItem::createFunction(it.second));
	// 	}

	// 	functionLookahead(m_script.lock()->getRootNode(), false);
	// }

	void VeloxInterpreter::setImporter(std::shared_ptr<IVeloxImporter> importer)
	{
		m_importer = importer;
	}

	bool VeloxInterpreter::hasHook(const std::string & name)
	{
		return m_hooks.find(name) != m_hooks.end();
	}

	void VeloxInterpreter::addHook(std::shared_ptr<VeloxHook> hook)
	{
		addHook(hook->getName(), hook);
	}

	void VeloxInterpreter::addHook(const std::string & name, std::shared_ptr<VeloxHook> hook)
	{
		if (hasHook(name)) {
			throw std::logic_error("ambiguous hook name '" + name + "'");
		}
		m_hooks[name] = hook;
	}

	void VeloxInterpreter::removeHook(const std::string & name)
	{
		m_hooks.erase(name);
	}

	void VeloxInterpreter::functionLookahead(const VeloxNode & node, bool innerFunction)
	{
		for (auto & child : node) {
			switch (child->getType()) {
				case VeloxNodeType::FUNCTION_DECLARATION:
					interpretFunctionDeclaration(*child, innerFunction);
					break;

				case VeloxNodeType::CLASS_DECLARATION:
					interpretClassDeclaration(*child);
					break;
			}
		}
	}

	void VeloxInterpreter::openScope()
	{
		m_scope = VeloxScope::create(std::move(m_scope));
	}

	void VeloxInterpreter::openBlockScope()
	{
		m_scope = VeloxScope::create(std::move(m_scope), true);
	}

	void VeloxInterpreter::closeScope()
	{
		m_scope = m_scope->detachFromParent();
	}

	void VeloxInterpreter::closeAllScopesButFirst()
	{
		while (m_scope->hasParent()) {
			m_scope = m_scope->detachFromParent();
		}
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluate(const VeloxNode & node)
	{
		switch (node.getType()) {
		case VeloxNodeType::STRING_LITERAL:
			return VeloxItem::createStringValue(node.getStringData());

		case VeloxNodeType::INTEGER_LITERAL:
			return VeloxItem::createIntValue(node.getIntData());

		case VeloxNodeType::REAL_LITERAL:
			return VeloxItem::createRealValue(node.getRealData());

		case VeloxNodeType::BOOL_LITERAL:
			return VeloxItem::createBoolValue(node.getIntData());

		case VeloxNodeType::FIELD_ACCESS:
			return evaluateFieldAccess(node, false);

		case VeloxNodeType::ARRAY_ACCESS:
			return evaluateArrayAccess(node);

		case VeloxNodeType::OBJECT_CREATION:
			return evaluateObjectCreation(node);

		case VeloxNodeType::SIMPLE_NAME:
			return evaluateSimpleName(node, false);

		case VeloxNodeType::ASSIGNMENT:
		case VeloxNodeType::ASSIGN_ADD:
		case VeloxNodeType::ASSIGN_SUB:
		case VeloxNodeType::ASSIGN_MUL:
		case VeloxNodeType::ASSIGN_DIV:
		case VeloxNodeType::ASSIGN_MOD:
			return evaluateAssignment(node);

		case VeloxNodeType::INCREMENT:
		case VeloxNodeType::DECREMENT:
		case VeloxNodeType::NOT:
			return evaluatePrefix(node);

		case VeloxNodeType::POST_INCREMENT:
		case VeloxNodeType::POST_DECREMENT:
			return evaluatePostfix(node);

		case VeloxNodeType::LESS_EQUAL:		
		case VeloxNodeType::LESS_THAN:		
		case VeloxNodeType::GREATER_EQUAL:	
		case VeloxNodeType::GREATER_THAN:	
		case VeloxNodeType::EQUAL:			
		case VeloxNodeType::NOT_EQUAL:			
			return evaluateRelation(node);

		case VeloxNodeType::UNARY_PLUS:		
		case VeloxNodeType::UNARY_MINUS:	
			return evaluateUnary(node);

		case VeloxNodeType::ADDITION:		
		case VeloxNodeType::SUBTRACTION:	
		case VeloxNodeType::MODULO:			
		case VeloxNodeType::MULTIPLICATION:	
		case VeloxNodeType::DIVISION:			
		case VeloxNodeType::LOG_AND:		
		case VeloxNodeType::LOG_OR:		
			return evaluateInfix(node);

		case VeloxNodeType::FUNCTION_CALL:
		{
			openBlockScope();
			auto result = evaluateFunctionCall(node);
			closeScope();
			return result;
		}

		case VeloxNodeType::ARRAY_INITIALIZER:
			return evaluateArrayInitializer(node);

		default:
			throw VeloxInterpreterException(std::string("internal interpreter error: ")
				+ "expression expected, got "
				+ node.toString());
		}
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluateInfix(const VeloxNode & node)
	{
		assert(node.numChildren() == 2);
		// TODO add shortcut evaluation

		try {
			std::shared_ptr<VeloxItem> result;
			switch (node.getType()) {
			case VeloxNodeType::ADDITION:
				return result = *evaluate(node[0]) + *evaluate(node[1]);

			case VeloxNodeType::SUBTRACTION:
				return result = *evaluate(node[0]) - *evaluate(node[1]);

			case VeloxNodeType::MODULO:
				return result = *evaluate(node[0]) % *evaluate(node[1]);

			case VeloxNodeType::MULTIPLICATION:
				return result = *evaluate(node[0]) * *evaluate(node[1]);

			case VeloxNodeType::DIVISION:
				return result = *evaluate(node[0]) / *evaluate(node[1]);

			case VeloxNodeType::LOG_AND: {
				auto leftResult = evaluate(node[0]);
				if (!leftResult->getBoolValue())
					return VeloxItem::createBoolValue(false);

				return VeloxItem::createBoolValue(evaluate(node[1])->getBoolValue());
			}

			case VeloxNodeType::LOG_OR: {
				auto leftResult = evaluate(node[0]);
				if (leftResult->getBoolValue())
					return VeloxItem::createBoolValue(true);

				return VeloxItem::createBoolValue(evaluate(node[1])->getBoolValue());
			}

			}
		}
		catch (const VeloxInterpreterException & e) {
			throw VeloxInterpreterException(e.getMessageText(), node.getPosition().getLine());
		}

		throw VeloxInterpreterException(std::string("internal interpreter error: ")
			+ "infix expression expected, got "
			+ node.toString());

	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluatePrefix(const VeloxNode & node)
	{
		assert(node.numChildren() == 1);
		auto item = evaluate(node[0]);

		switch (node.getType()) {
		case VeloxNodeType::INCREMENT:
			return ++*item;

		case VeloxNodeType::DECREMENT:
			return --*item;

		case VeloxNodeType::NOT:
			return !*item;

		default:
			throw VeloxInterpreterException(std::string("internal interpreter error: ")
				+ "prefix expression expected, got "
				+ node.toString());
		}
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluatePostfix(const VeloxNode & node)
	{
		assert(node.numChildren() == 1);
		auto item = evaluate(node[0]);

		switch (node.getType()) {
		case VeloxNodeType::POST_INCREMENT:
			return (*item)++;

		case VeloxNodeType::POST_DECREMENT:
			return (*item)--;

		default:
			throw VeloxInterpreterException(std::string("internal interpreter error: ")
				+ "postfix expression expected, got "
				+ node.toString());
		}
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluateUnary(const VeloxNode & node)
	{
		assert(node.numChildren() == 1);

		auto item = evaluate(node[0]);
		switch (node.getType()) {
		case VeloxNodeType::UNARY_PLUS:
			return +*item;

		case VeloxNodeType::UNARY_MINUS:
			return -*item;

		default:
			throw VeloxInterpreterException(std::string("internal interpreter error: ")
				+ "unary operation statement expected, got "
				+ node.toString());
		}
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluateAssignment(const VeloxNode & node)
	{
		assert(node.numChildren() == 2);

		try {
			auto left = evaluateLeftHandSide(node.getChild(0));
			auto right = evaluate(node.getChild(1));
			switch (node.getType()) {

			case VeloxNodeType::ASSIGNMENT:
				*left = *right;
				break;

			case VeloxNodeType::ASSIGN_ADD:
				*left += *right;
				break;

			case VeloxNodeType::ASSIGN_SUB:
				*left -= *right;
				break;

			case VeloxNodeType::ASSIGN_DIV:
				*left /= *right;
				break;

			case VeloxNodeType::ASSIGN_MUL:
				*left *= *right;
				break;

			case VeloxNodeType::ASSIGN_MOD:
				*left %= *right;
				break;

			default:
				throw VeloxInterpreterException(std::string("internal interpreter error: ")
					+ "assignment operation expected, got "
					+ node.toString());
			}

			return left;
		}
		catch (const VeloxInterpreterException & e) {
			if (e.isLocated())
				throw e;
			else
				throw VeloxInterpreterException(e.getMessageText(), node.getPosition().getLine());
		}

	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluateRelation(const VeloxNode & node)
	{
		assert(node.numChildren() == 2);
		auto left = evaluate(node[0]);
		auto right = evaluate(node[1]);

		try {
			std::shared_ptr<VeloxItem> result;
			switch (node.getType()) {
			case VeloxNodeType::LESS_THAN:
				return *left < *right;

			case VeloxNodeType::LESS_EQUAL:
				return *left <= *right;

			case VeloxNodeType::GREATER_THAN:
				return *left > *right;

			case VeloxNodeType::GREATER_EQUAL:
				return *left >= *right;

			case VeloxNodeType::EQUAL:
				return *left == *right;

			case VeloxNodeType::NOT_EQUAL:
				return *left != *right;

			default:
				throw VeloxInterpreterException(std::string("internal interpreter error: ")
					+ "infix relation statement expected, got " 
					+ node.toString());
			}
		}
		catch (const VeloxInterpreterException & e) {
			throw VeloxInterpreterException(e.getMessageText(), node.getPosition().getLine());
		}
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluateLeftHandSide(const VeloxNode & node)
	{
		switch (node.getType()) {
		case VeloxNodeType::GLOBAL:
			return evaluateGlobalSimpleName(node);

		case VeloxNodeType::SIMPLE_NAME:
			return evaluateSimpleName(node, true);

		case VeloxNodeType::FIELD_ACCESS:
			return evaluateFieldAccess(node, true);

		case VeloxNodeType::ARRAY_ACCESS:
			return evaluateArrayAccess(node);

		default:
			throw VeloxInterpreterException("internal error, illegal left-hand-side expression '" 
				+ node.toString() + "'", node.getPosition().getLine());
		}
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluateArrayInitializer(const VeloxNode & node)
	{
		assert(node.getType() == VeloxNodeType::ARRAY_INITIALIZER);
		auto result = VeloxItem::createArray();

		for (auto & elem : node) {
			result->addArrayElem(evaluate(*elem));
		}

		return result;
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluateFieldAccess(const VeloxNode & node, bool leftHandSide)
	{
		assert(node.getType() == VeloxNodeType::FIELD_ACCESS);
		assert(node.numChildren() == 1);
		
		auto item = evaluate(node[0]);

		if (m_scope->isBlock()) {
			// Keep this item alive (as anonymous item) until the statement has been executed,
			// otherwise functions would loose there surrounding object.
			m_scope->putItem(item); // make an anonymous item
		}

		if (item->hasAttribute(node.getStringData())) {
			return item->getAttribute(node.getStringData());
		}
		else if (leftHandSide) {
			auto result = VeloxItem::createUndefined();
			item->addAttribute(node.getStringData(), result);
			return result;
		}
		else {
			throw VeloxInterpreterException("attribute '" + node.getStringData() + "' is unknown", node[0].getPosition().getLine());
		}
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluateArrayAccess(const VeloxNode & node)
	{
		assert(node.getType() == VeloxNodeType::ARRAY_ACCESS);
		assert(node.numChildren() == 2);

		auto item = evaluate(node[0]);
		auto index = evaluate(node[1]);
		
		try {
			return item->getArrayElem(index->getIntValue());
		}
		catch (const VeloxInterpreterException & e) {
			throw VeloxInterpreterException(e.getMessageText(), node.getPosition().getLine());
		}
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluateSimpleName(const VeloxNode & node, bool leftHandSide)
	{
		assert(node.getType() == VeloxNodeType::SIMPLE_NAME);

		auto item = m_scope->findItem(node.getStringData());

		if (item) {
			if (leftHandSide && !m_scope->hasLocalItem(node.getStringData())) {
				//emitWarning("local variable " + node.getStringData() + " is hiding global");
				item = VeloxItem::createUndefined();
				m_scope->putItem(node.getStringData(), item);
			}
		}
		else {
			if (leftHandSide) {
				item = VeloxItem::createUndefined();
				m_scope->putItem(node.getStringData(), item);
			}
			else {
				throw VeloxInterpreterException(node.getStringData() + " is not defined", node.getPosition().getLine());
			}
		}

		return item;
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluateGlobalSimpleName(const VeloxNode & node)
	{
		assert(node.getType() == VeloxNodeType::GLOBAL);
		
		auto item = m_scope->getRoot().findItem(node[0].getStringData());
		if (!item) {
			item = VeloxItem::createUndefined();
			m_scope->getRoot().putItem(node[0].getStringData(), item);
		}

		return item;
	}

	void VeloxInterpreter::interpretPrint(const VeloxNode & node)
	{
		assert(node.getType() == VeloxNodeType::PRINT);
		assert(node.numChildren() == 1);
		if (!m_printFunc)
			return;

		auto item = evaluate(node[0]);
		m_printFunc(item->getStringValue(*this));
	}

	//std::string VeloxInterpreter::itemToString(const VeloxItem & item)
	//{
	//	switch (item.getType()) {
	//	case VeloxItemType::INT:
	//	case VeloxItemType::REAL:
	//	case VeloxItemType::BOOL:
	//	case VeloxItemType::STRING:
	//	case VeloxItemType::FUNCTION:
	//	case VeloxItemType::HOOK:
	//		return item.getStringValue();

	//	case VeloxItemType::ARRAY: {
	//		std::string result = "[";
	//		const auto & elems = item.getArrayElems();
	//		for (auto it = elems.begin(); it != elems.end(); ) {
	//			result += itemToString(**it++);
	//			if (it != elems.end())
	//				result += ", ";
	//		}
	//		result += "]";
	//		return result;
	//	}

	//	case VeloxItemType::REFERENCE:
	//	case VeloxItemType::OBJECT: {
	//		if (item.hasAttribute("toString")) {
	//			auto attr = item.getAttribute("toString");
	//			switch (attr->getType()) {
	//			case VeloxItemType::HOOK:
	//			case VeloxItemType::FUNCTION:
	//				return itemToString(*evaluate(*attr));
	//			}
	//		}
	//		return "OBJ";
	//	}

	//	default:
	//		throw VeloxInterpreterException("Internal interpreter error");
	//	}

	//}

	auto VeloxInterpreter::interpretBlock(const VeloxNode & node) -> ExitType
	{
		openBlockScope();
		for (auto it = node.begin(); it != node.end(); ++it) {
			ExitType exitType;
			switch (exitType = interpretStatement(**it)) {

			case ExitType::VOID:
				break;

			default:
				closeScope();
				return exitType;
			}
		}

		closeScope();
		return ExitType::VOID;
	}

	auto VeloxInterpreter::interpretSequence(const VeloxNode & node) -> ExitType
	{
		for (auto it = node.begin(); it != node.end(); ++it) {
			ExitType exitType;
			switch (exitType = interpretStatement(**it)) {

			case ExitType::VOID:
				break;

			default:
				return exitType;
			}
		}

		return ExitType::VOID;
	}


	auto VeloxInterpreter::interpretIf(const VeloxNode & node) -> ExitType
	{
		assert(node.getType() == VeloxNodeType::IF);
		assert(node.numChildren() >= 2);

		auto cond = evaluate(node[0]);
		if (cond->getType() != VeloxItemType::BOOL) {
			throw VeloxInterpreterException("boolean expression expected");
		}
		if (cond->getBoolValue()) {
			return interpretStatement(node[1]);
		}
		else if (node.numChildren() >= 3) {
			return interpretStatement(node[2]);
		}

		return ExitType::VOID;
	}

	void VeloxInterpreter::interpretReturn(const VeloxNode & node)
	{
		assert(node.getType() == VeloxNodeType::RETURN);

		if (node.numChildren() > 0) {
			std::shared_ptr<VeloxItem> result;
			result = evaluate(node[0]);
			peekReturn() = *result;
			if (result->getType() == VeloxItemType::FUNCTION && result->isInnerFunction()) {
				result->setClosure(m_scope->createClosure());
			}
		}

	}

	auto VeloxInterpreter::interpretWhile(const VeloxNode & node) -> ExitType
	{
		assert(node.getType() == VeloxNodeType::WHILE);
		assert(node.numChildren() == 2);

		auto cond = evaluate(node[0]);
		if (cond->getType() != VeloxItemType::BOOL) {
			throw VeloxInterpreterException("boolean expression expected");
		}

		while (cond->getBoolValue()) {
			switch (interpretStatement(node[1])) {
			case ExitType::BREAK:
				return ExitType::VOID;

			case ExitType::RETURN:
				return ExitType::RETURN;
			}
			cond = evaluate(node[0]);
		}

		return ExitType::VOID;
	}

	auto VeloxInterpreter::interpretFor(const VeloxNode & node) -> ExitType
	{
		assert(node.getType() == VeloxNodeType::FOR);
		assert(node.numChildren() == 4);
		openBlockScope();

		for (auto & init : node[0]) {
			interpretExpressionStatement(*init);
		}

		auto cond = evaluate(node[1]);
		if (cond->getType() != VeloxItemType::BOOL) {
			throw VeloxInterpreterException("boolean expression expected");
		}

		while (cond->getBoolValue()) {
			switch (interpretStatement(node[3])) {
			case ExitType::BREAK:
				closeScope();
				return ExitType::VOID;

			case ExitType::RETURN:
				closeScope();
				return ExitType::RETURN;
			}

			for (auto & updater : node[2]) {
				interpretExpressionStatement(*updater);
			}

			cond = evaluate(node[1]);
		}

		closeScope();
		return ExitType::VOID;
	}
 
	auto VeloxInterpreter::interpretStatement(const VeloxNode & node) -> ExitType
	{
		switch (node.getType()) {

		case VeloxNodeType::FUNCTION_DECLARATION:
			// Ignore due to function lookahead.
			return ExitType::VOID;

		case VeloxNodeType::CLASS_DECLARATION:
			// Ignore due to function lookahead.
			return ExitType::VOID;

		case VeloxNodeType::BLOCK:
			RETURN_VERIFIED(interpretBlock(node));
			//return interpretBlock(node);

		case VeloxNodeType::SEQUENCE:
			RETURN_VERIFIED(interpretSequence(node));
			//return interpretSequence(node);

		case VeloxNodeType::RETURN:
			interpretReturn(node);
			return ExitType::RETURN;

		case VeloxNodeType::IF:
			return interpretIf(node);

		case VeloxNodeType::WHILE:
			RETURN_VERIFIED(interpretWhile(node));
			//return interpretWhile(node);

		case VeloxNodeType::FOR:
			RETURN_VERIFIED(interpretFor(node));
			//return interpretFor(node);

		case VeloxNodeType::PRINT:
			interpretPrint(node);
			return ExitType::VOID;

		case VeloxNodeType::IMPORT:
			return interpretImport(node);

		case VeloxNodeType::ASSIGNMENT:
		case VeloxNodeType::ASSIGN_ADD:
		case VeloxNodeType::ASSIGN_SUB:
		case VeloxNodeType::ASSIGN_DIV:
		case VeloxNodeType::ASSIGN_MUL:
		case VeloxNodeType::ASSIGN_MOD :
		case VeloxNodeType::INCREMENT:
		case VeloxNodeType::DECREMENT:
		case VeloxNodeType::SIMPLE_NAME:
		case VeloxNodeType::FIELD_ACCESS:
		case VeloxNodeType::FUNCTION_CALL:
		case VeloxNodeType::OBJECT_CREATION:
			interpretExpressionStatement(node);
			return ExitType::VOID;

		default:
			throw VeloxInterpreterException(std::string("internal interpreter error: ")
				+ "unexpected node " + node.toString());
		}
	}

	auto VeloxInterpreter::interpretImport(const VeloxNode & node) -> ExitType
	{
		assert(node.getType() == VeloxNodeType::IMPORT);
		assert(node.numChildren() == 0);

		auto importer = m_importer.lock();
		if (!importer) {
			throw VeloxInterpreterException("unable to execute import statement, no importer defined");
		}

		try {
			auto ast = importer->importAst(node.getStringData(), m_sourcePath);
			if (!ast) {
				throw VeloxInterpreterException("unable to import script '" + node.getStringData() + ": file not found");
			}
			functionLookahead(*ast, false);
			return interpretSequence(*ast);
		}
		catch (std::runtime_error & e) {
			throw VeloxInterpreterException("error importing script '" + node.getStringData() + "': " + e.what());
		}
	}

	void VeloxInterpreter::interpretExpressionStatement(const VeloxNode & node)
	{
		evaluate(node);
	}

	void VeloxInterpreter::interpretFunctionDeclaration(const VeloxNode & node, bool innerFunction)
	{
		assert(node.getType() == VeloxNodeType::FUNCTION_DECLARATION);
		assert(node.numChildren() == 2);
		if (m_scope->hasLocalItem(node.getStringData())) {
			throw VeloxInterpreterException("ambiguous function name " + node.getStringData());
		}
		m_scope->putItem(node.getStringData(), VeloxItem::createFunction(node.shared_from_this(), innerFunction));
	}

	void VeloxInterpreter::interpretClassDeclaration(const VeloxNode & node)
	{
		assert(node.getType() == VeloxNodeType::CLASS_DECLARATION);
		if (m_scope->hasLocalItem(node.getStringData())) {
			throw VeloxInterpreterException("ambiguous class name " + node.getStringData());
		}
		m_scope->putItem(node.getStringData(), VeloxItem::createClass(node.shared_from_this()));
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluateFunctionCall(const VeloxNode & node)
	{
		assert(node.getType() == VeloxNodeType::FUNCTION_CALL);
		assert(node.numChildren() == 2);

		auto func = evaluate(node[0]);

		switch (func->getXType()) {
		case VeloxItemType::FUNCTION:
			return evaluateScriptFunctionCall(*func, node[1]);

		case VeloxItemType::REFERENCE:

			return evaluateScriptFunctionCall(*func, node[1]);

		case VeloxItemType::HOOK:
			try {
				return evaluateHookFunctionCall(*func, node[1]);
			}
			catch (const VeloxInterpreterException & e) {
				if (!e.isLocated()) {
					throw VeloxInterpreterException(e.getMessageText(), node.getPosition().getLine());
				}
				throw;
			}

		default:
			throw VeloxInterpreterException("not a function");
		}
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluateObjectCreation(const VeloxNode & node)
	{
		auto clazz = m_scope->findItem(node.getStringData());
		if (!clazz) {
			throw VeloxInterpreterException("unknown class '" + node.getStringData() + "'");
		}
		auto result = VeloxItem::createObject();

		// Add methods/functions.
		for (auto & func : clazz->getAstNode()) {
			result->addAttribute(func->getStringData(), VeloxItem::createFunction(func));
		}

		if (result->hasAttribute(node.getStringData())) {
			auto item = result->getAttribute(node.getStringData());
			if (item->getType() == VeloxItemType::FUNCTION) {
				auto params = VeloxNode::create(VeloxNodeType::FUNCTION_PARAMS);
				evaluateScriptFunctionCall(*item, node[0]);
			}
		}

		return result;
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluateScriptFunctionCall(const VeloxItem & item, const VeloxNode & params)
	{
		assert(item.getXType() == VeloxItemType::FUNCTION);
		assert(item.getAstNode().numChildren() == 2);

		// function parameters must be added to isolated scope in order not
		// to mask parameters names used for parameter evaluation.
		auto funcScope = VeloxScope::create();

		size_t idx = 0;
		for (const auto & formParam : item.getAstNode()[0]) {
			assert(formParam->getType() == VeloxNodeType::SIMPLE_NAME);

			auto param = VeloxItem::createUndefined();
			if (params.numChildren() > idx) {
				*param = *evaluate(params[idx]);
			}
			funcScope->putItem(formParam->getStringData(), param);

			++idx;
		}
		if (item.hasParent()) {
			funcScope->putItem("this", item.getParent());
		}

		// Make function scope the current scope.
		funcScope->setParent(std::move(m_scope));
		m_scope = std::move(funcScope);

		if (item.hasClosure()) {
			item.getClosure().injectItems(*m_scope);
		}

		pushReturn(VeloxItem::createUndefined());
		functionLookahead(item.getAstNode()[1], true);
		interpretStatement(item.getAstNode()[1]);
		closeScope(); 

		return popReturn();
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::evaluateHookFunctionCall(const VeloxItem & item, const VeloxNode & params)
	{
		assert(item.getType() == VeloxItemType::HOOK);

		// function parameters must be added to isolated scope in order not
		// to mask parameters names used for parameter evaluation.
		auto funcScope = VeloxScope::create();

		size_t idx = 0;
		for (const auto & formParam : item.getHook()) {
			if (params.numChildren() > idx) {
				auto item = evaluate(params[idx]);
				if (item->getType() == VeloxItemType::FUNCTION && item->isInnerFunction()) {
					item->setClosure(m_scope->createClosure());
				}
				funcScope->putItem(formParam, item);
			}
			else {
				funcScope->putItem(formParam, VeloxItem::createUndefined());
			}

			++idx;
		}
		if (item.hasParent()) {
			funcScope->putItem("this", item.getParent());
		}

		// Make function scope the current scope.
		funcScope->setParent(std::move(m_scope));
		m_scope = std::move(funcScope);

		auto result = item.getHook().invoke(m_rootNode, *m_scope);
		closeScope();
		return result;
	}

	void VeloxInterpreter::emitWarning(const std::string & msg)
	{
		if (m_warnFunc) {
			m_warnFunc(msg);
		}
	}

	void VeloxInterpreter::addHooks(VeloxScope & scope)
	{
		for (auto & it : m_hooks) {
			scope.putItem(it.first, VeloxItem::createHook(it.second));
		}
	}

	std::shared_ptr<VeloxItem> VeloxInterpreter::popReturn()
	{
		if (m_returnStack.empty()) {
			throw VeloxInterpreterException(std::string("internal interpreter error: ")
				+ "return stack underrun");
		}

		auto result = m_returnStack.back();
		m_returnStack.pop_back();
		return result;
	}

	void VeloxInterpreter::pushReturn(std::shared_ptr<VeloxItem> item)
	{
		m_returnStack.push_back(item);
	}

	VeloxItem & VeloxInterpreter::peekReturn()
	{
		if (m_returnStack.empty()) {
			throw VeloxInterpreterException(std::string("internal interpreter error: ")
				+ "return stack underrun");
		}

		return *m_returnStack.back();
	}



} // end of namespace