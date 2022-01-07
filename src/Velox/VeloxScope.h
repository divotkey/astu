/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/VeloxItem.h"
#include "Util/StringUtils.h"
#include "Util/Pooled.h"

// C++ Standard Library includes
#include <string>
#include <memory>
#include <unordered_map>

namespace astu {

	class VeloxScope : public Pooled<VeloxScope> {
	public:

		static std::unique_ptr<VeloxScope> create(std::unique_ptr<VeloxScope> parent = nullptr, bool block = false) {
			return std::unique_ptr<VeloxScope>(new VeloxScope(std::move(parent), block));
		}

		/**
		 * Returns whether this scope has a parent scope.
		 *
		 * @return `true` if this scope has a parent scope.
		 */
		bool hasParent() const;

		/**
		 * Sets the parent scope of this scope.
		 *
		 * @param parent	the parent scope
		 */
		void setParent(std::unique_ptr<VeloxScope> parent);

		/**
		 * Detaches this scope from its parent.
		 * 
		 * @return the parent of this scope
		 */
		std::unique_ptr<VeloxScope> detachFromParent();

		/**
		 * Returns the root scope.
		 *
		 * @param the root scope
		 */
		VeloxScope & getRoot();

		/**
		 * Returns the scope level.
		 * The scope level describes the number of scopes hierarchies, e.g.,
		 * the root scope has level 0.
		 *
		 * @returnt the scope level
		 */
		size_t getLevel();

		std::unique_ptr<VeloxScope> createClosure();
		void injectItems(VeloxScope& scope) const;

		bool hasItem(const std::string & name) const;
		bool hasLocalItem(const std::string & name) const;
		std::shared_ptr<VeloxItem> getItem(const std::string & name);
		std::shared_ptr<const VeloxItem> getItem(const std::string & name) const;
		std::shared_ptr<VeloxItem> findItem(const std::string & name);
		std::shared_ptr<const VeloxItem> findItem(const std::string & name) const;
		void putItem(const std::string & name, std::shared_ptr<VeloxItem> item);

		/**
		 * Adds an anonymous item to this scope.
		 * Anonymous items are usually temporary items during the evaluation of an expression.
		 *
		 * @param the anonymous item to be added
		 */
		void putItem(std::shared_ptr<VeloxItem> item);

		void clear();
		
		bool isBlock() const {
			return m_block;
		}

		int getIntValue(const std::string & name) const;
		bool getBoolValue(const std::string & name) const;
		std::string getStringValue(const std::string & name) const;
		double getDoubleValue(const std::string & name) const;
		Real getRealValue(const std::string & name) const;

		/**
		 * Returns the integer value of the specified item.
		 * If the specified item is  undefined, the given default value is returned.
		 *
		 * @param name	the name of the item
		 * @param defaultValue	the default value
		 * @throws VeloxInterpreterException in case the item does not exist or does not contain
		 *		integer data
		 */
		int getIntValue(const std::string & name, int defaultValue) const;

		int getIntLowerBounded(const std::string & name,int minValue, int defaultValue) const;

		Real getRealLowerBounded(const std::string & name, Real minValue, Real defaultValue) const;

		/**
		 * Returns the real value of the specified item.
		 * If the specified item is  undefined, the given default value is returned.
		 *
		 * @param name	the name of the item
		 * @param defaultValue	the default value
		 * @throws VeloxInterpreterException in case the item does not exist or does not contain
		 *		integer data
		 */
		Real getRealValue(const std::string & name, Real defaultValue) const;

		/**
		 * Returns the string value of the specified item.
		 * If the specified item is  undefined, the given default value is returned.
		 *
		 * @param name	the name of the item
		 * @param defaultValue	the default value
		 * @throws VeloxInterpreterException in case the item does not exist or does not contain
		 *		integer data
		 */
		std::string getStringValue(const std::string & name, const std::string & defaultValue) const;

		Real getRealValueWithinRange(const std::string & name, Real min, Real max, Real defaultValue = 0);

		/**
		 * Returns whether the specified item is of type undefined.
		 *
		 * @param the name of the item to test
		 * @return `true` if the item is of type undefined
		 */
		bool isUndefined(const std::string & name) {
			return isType(name, VeloxItemType::UNDEFINED);
		}

		/**
		 * Returns whether the specified item is of a type certain type.
		 *
		 * @param the name of the item to test
		 * @param the velox type to test for
		 * @return `true` if the item is of the specified type
		 */
		bool isType(const std::string & name, VeloxItemType type) {
			return getItem(name)->getType() == type;
		}

		template <typename T>
		std::shared_ptr<T> getAttachable(const std::string & name, const std::string & errorText) {
			auto result = getItem(name)->findAttachable<T>();
			if (!result) {
				throw VeloxInterpreterException(errorText);
			}
			return result;
		}

		template <typename T>
		std::shared_ptr<T> getAttachable(const std::string & name) {
			return getItem(name)->getAttachable<T>();
		}

		template <typename T>
		bool hasAttachable(const std::string & name) {
			return getItem(name)->hasAttachable<T>();
		}

	private:
		/** The parent scope of this scope. */
		std::unique_ptr<VeloxScope> m_parent;

		/** The items stored in this scope. */
		std::unordered_map<std::string, std::shared_ptr<VeloxItem>> m_items;

		/** The anonymous items of this scope. */
		std::vector <std::shared_ptr<VeloxItem>> m_anonymousItems;

		/** Indicates whether this is a block scope. */
		bool m_block;

		/**
		 * Constructor.
		 *
		 * @param parent	the parent scope of this scope
		 * @param block		`true` to make a block scope
		 */
		VeloxScope(std::unique_ptr<VeloxScope> parent, bool block);

		// Disable copy and assignment.
		VeloxScope(const VeloxScope&) = delete;
		VeloxScope & operator=(const VeloxScope&) = delete;
	};

} // end of namespace