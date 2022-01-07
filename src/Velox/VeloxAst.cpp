
/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
// #include "Logger.h"
#include "VeloxAst.h"

// C++ Standard Library includes
#include <cassert>
#include <algorithm>
#include <stdexcept>

namespace astu {

	const std::vector<std::string> VeloxNode::TYPE_NAMES = {
		"Assignment", "Expression", "SimpleName", "IntegerLiteral", "StringLiteral",
		"LessThan", "LessEqual", "GreaterThan", "GreaterEqual", "Equal",
		"NotEqual", "RealLiteral", "BoolLiteral", "Addition", "Subtraction",
		"Modulo", "Multiplication", "AssignAdd", "AssignSub", "AssignMul",
		"AssignDiv", "AssignMod", "UnaryMinus", "UnaryPlus", "LogAnd",
		"LogOr", "Not", "Increment", "Decrement", "PostIncrement",
		"PostDecrement", "ArrayInitializer", "ArrayAccess", "Division", "Print",
		"Block", "For", "If", "Else", "While",
		"Return", "ForInitializer", "ForUpdater", "Sequence", "Global",
		"Import", "FunctionDeclaration", "FormalParameter", "FunctionCall", "FunctionParams",
		"Hook", "ClassDeclaration", "New", "ObjectCreation", "FieldAccess"
	};

	VeloxNode::VeloxNode(VeloxNodeType type)
		: m_type(type)
	{
		m_position.clear();
		PREPARE_POOLING();
	}

	const std::string & VeloxNode::toString(VeloxNodeType type)
	{
		int idx = static_cast<int>(type);
		if (idx < 0 || idx >= TYPE_NAMES.size()) {
			throw std::logic_error("Unable to interpret node type (" + std::to_string(idx) + ")");
		}

		return TYPE_NAMES[idx];
	}

	void VeloxNode::addChild(std::shared_ptr<VeloxNode> child)
	{
		assert(!hasChild(child));
		m_children.push_back(child);
	}

	bool VeloxNode::hasChild(std::shared_ptr<VeloxNode> child) const
	{
		return std::find(m_children.begin(), m_children.end(), child) != m_children.end();
	}

	void VeloxNode::removeChildren()
	{
		m_children.clear();
	}

	size_t VeloxNode::numChildren() const
	{
		return m_children.size();
	}

	VeloxNode & VeloxNode::getChild(size_t idx)
	{
		return *m_children.at(idx);
	}

	const VeloxNode & VeloxNode::getChild(size_t idx) const
	{
		return *m_children.at(idx);
	}

	std::vector<std::shared_ptr<VeloxNode>>& VeloxNode::getChildren()
	{
		return m_children;
	}

	std::vector<std::shared_ptr<VeloxNode>>::iterator VeloxNode::begin()
	{
		return m_children.begin();
	}

	std::vector<std::shared_ptr<VeloxNode>>::iterator VeloxNode::end()
	{
		return m_children.end();
	}
	
	std::vector<std::shared_ptr<VeloxNode>>::const_iterator VeloxNode::begin() const
	{
		return m_children.cbegin();
	}

	std::vector<std::shared_ptr<VeloxNode>>::const_iterator VeloxNode::end() const
	{
		return m_children.cend();
	}

	VeloxNode & VeloxNode::operator[](size_t index)
	{
		return *m_children[index];
	}

	const VeloxNode & VeloxNode::operator[](size_t index) const
	{
		return *m_children[index];
	}

	VeloxNode & VeloxNode::at(size_t index)
	{
		return *m_children.at(index);
	}

	const VeloxNode & VeloxNode::at(size_t index) const
	{
		return *m_children.at(index);
	}

	void VeloxNode::setStringData(const std::string & data)
	{
		m_stringData = data;
	}

	void VeloxNode::setIntegerData(int data)
	{
		m_integerData = data;
	}

	void VeloxNode::setRealData(double data)
	{
		m_realData = data;
	}

	void VeloxNode::clearData()
	{
		m_integerData = 0;
		m_stringData.clear();
	}

} // end of namespace