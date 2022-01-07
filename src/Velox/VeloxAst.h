
/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Util/Pooled.h"
#include "Velox/VeloxScanner.h"

// C++ Standard Library includes
#include <memory>
#include <vector>
#include <string>

namespace astu {

	// Forward declaration.
	class VeloxHook;

	enum class VeloxNodeType {
		ASSIGNMENT, EXPRESSION, SIMPLE_NAME, INTEGER_LITERAL, STRING_LITERAL,
		LESS_THAN, LESS_EQUAL, GREATER_THAN, GREATER_EQUAL, EQUAL,
		NOT_EQUAL, REAL_LITERAL, BOOL_LITERAL, ADDITION, SUBTRACTION,
		MODULO, MULTIPLICATION, ASSIGN_ADD, ASSIGN_SUB, ASSIGN_MUL,
		ASSIGN_DIV, ASSIGN_MOD, UNARY_MINUS, UNARY_PLUS, LOG_AND,
		LOG_OR, NOT, INCREMENT, DECREMENT, POST_INCREMENT,
		POST_DECREMENT, ARRAY_INITIALIZER, ARRAY_ACCESS, DIVISION, PRINT,
		BLOCK, FOR, IF, ELSE, WHILE,
		RETURN, FOR_INITIALIZER, FOR_UPDATER, SEQUENCE, GLOBAL,
		IMPORT, FUNCTION_DECLARATION, FORMAL_PARAMETER, FUNCTION_CALL, FUNCTION_PARAMS,
		HOOK, CLASS_DECLARATION, NEW, OBJECT_CREATION, FIELD_ACCESS
	};

	class VeloxNode final : public Pooled<VeloxNode>, public std::enable_shared_from_this<VeloxNode> {
	public:

		static std::shared_ptr<VeloxNode> create(VeloxNodeType type) {
			return std::shared_ptr<VeloxNode>(new VeloxNode(type));
		}

		/**
		 * Returns a string representation of the specified node type.
		 *
		 * @param type	the node type
		 * @return the string representation of the specified node type
		 */
		static const std::string & toString(VeloxNodeType type);

		/**
		 * Returns the type of this node.
		 *
		 * @return the type of this node
		 */
		VeloxNodeType getType() const {
			return m_type;
		}

		/**
		 * Adds the specified node as child to this node.
		 *
		 * @param child	the child node to add
		 */
		void addChild(std::shared_ptr<VeloxNode> child);

		/**
		 * Returns whether the specified node is a direct child node of this node.
		 *
		 * @param child	the node to test
		 * @return `true` if the specified node is a direct child of this node
		 */
		bool hasChild(std::shared_ptr<VeloxNode> child) const;

		/**
		 * Removes all children from this node.
		 */
		void removeChildren();

		/**
		 * Returns the number of child nodes.
		 *
		 * @return the number of child nodes.
		 */
		size_t numChildren() const;

		/**
		 * Return the child node with the specified index.
		 *
		 * @param index	the index of the child node to return
		 * @return the child node with the specified index
		 * @throws std::out_of_range in case the specified index is invalid
		 */
		VeloxNode & getChild(size_t index);

		/**
		 * Return the child node with the specified index.
		 *
		 * @param index	the index of the child node to return
		 * @return the child node with the specified index
		 * @throws std::out_of_range in case the specified index is invalid
		 */
		const VeloxNode & getChild(size_t index) const;

		/**
		 * Returns the children of this node.
		 *
		 * @return vector holding the child nodes of this node.
		 */
		std::vector<std::shared_ptr<VeloxNode>> & getChildren();

		/**
		 * Returns an iterator pointing to the first child node.
		 *
		 * @return iterator pointing at the first child node.
		 */
		std::vector<std::shared_ptr<VeloxNode>>::iterator begin();

		/**
		 * Returns an iterator referring to the past-the-end element in the child container.
		 *
		 * @return iterator to the element past the end of the child sequence
		 */
		std::vector<std::shared_ptr<VeloxNode>>::iterator end();

		/**
		 * Returns an iterator pointing to the first child node.
		 *
		 * @return iterator pointing at the first child node
		 */
		std::vector<std::shared_ptr<VeloxNode>>::const_iterator begin() const;

		/**
		 * Returns an iterator referring to the past-the-end element in the child container.
		 *
		 * @return iterator to the element past the end of the child sequence
		 */
		std::vector<std::shared_ptr<VeloxNode>>::const_iterator end() const;

		/**
		 * Subscript operator for this node.
		 *
		 * @param index	of the child node to be retrieved
		 * @return the requested child node
		 */
		VeloxNode& operator[](size_t index);

		/**
		 * Subscript operator for this node.
		 *
		 * @param index	of the child node to be retrieved
		 * @return the requested child node
		 */
		const VeloxNode& operator[](size_t index) const;

		/**
		 * Accesses the child node with the specified index.
		 *
		 * @param index	of the element to be retrieved
		 * @return the requested child node
		 * @throws std::out_of_range in case the specified index is invalid
		 */
		VeloxNode& at(size_t index);

		/**
		 * Accesses the child node with the specified index.
		 *
		 * @param index	of the element to be retrieved
		 * @return the requested child node
		 * @throws std::out_of_range in case the specified index is invalid
		 */
		const VeloxNode& at(size_t index) const;

		/**
		 * Sets the optional string data for this node.
		 *
		 * @param data	the string data to attach
		 */
		void setStringData(const std::string & data);

		/**
		 * Returns the string data attached to this node.
		 *
		 * @return the string data
		 */
		const std::string & getStringData() const {
			return m_stringData;
		}

		/**
		 * Sets the optional integer data for this node.
		 *
		 * @param data	the integer data to attach
		 */
		void setIntegerData(int data);

		/**
		 * Returns the integer data attached to this node.
		 *
		 * @return the integer data
		 */
		int getIntData() const {
			return m_integerData;
		}

		/**
		 * Sets the optional real data for this node.
		 *
		 * @param data	the real data to attach
		 */
		void setRealData(double data);

		/**
		 * Returns the real data attached to this node.
		 *
		 * @return the real data
		 */
		double getRealData() const {
			return m_realData;
		}

		/** 
		 * Returns a string representation of this node.
		 *
		 * @return the string representation of this node's type
		 */
		const std::string & toString() const {
			return toString(m_type);
		}

		void setPosition(const TokenPosition & position) {
			m_position = position;
		}

		const TokenPosition & getPosition() const {
			return m_position;
		}

	private:
		/** The human readable names of the node types. */
		static const std::vector<std::string> TYPE_NAMES;

		/** The type of this node. */
		VeloxNodeType m_type;

		/** The child nodes of this node. */
		std::vector<std::shared_ptr<VeloxNode>> m_children;

		/** Optional string data stored with this node. */
		std::string m_stringData;

		/** Optional integer data stored with this node. */
		int m_integerData;

		/** Optional integer data stored with this node. */
		double m_realData;

		/** The position of this node within the source code. */
		TokenPosition m_position;

		/**
		 * Private constructor.
		 *
		 * @param type	the type of this node
		 */
		VeloxNode(VeloxNodeType type);

		// Disable copy and assignment.
		VeloxNode(const VeloxNode&) = delete;
		VeloxNode & operator=(const VeloxNode&) = delete;

		/**
		 * Clears all data attached to this node.
		 */
		void clearData();
	};

} // end of namespace