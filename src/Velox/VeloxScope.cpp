
/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// REMOVE WHEN REFACTORING IS DONE
using Real = float;

// Local includes
#include "Velox/VeloxScope.h"
#include "Velox/VeloxInterpreterException.h"
#include "Velox/VeloxItem.h"
#include "Util/StringUtils.h"

// C++ Standard Library includes
#include <cassert>
#include <stdexcept>
 
namespace astu {

	VeloxScope::VeloxScope(std::unique_ptr<VeloxScope> parent, bool block)
		: m_parent(std::move(parent)), m_block(block)
	{
		// Intentionally left empty.
	}

	bool VeloxScope::hasParent() const
	{
		return m_parent != nullptr;
	}

	void VeloxScope::setParent(std::unique_ptr<VeloxScope> parent)
	{
		m_parent = std::move(parent);
	}

	std::unique_ptr<VeloxScope> VeloxScope::detachFromParent()
	{
		return std::move(m_parent);
	}

	VeloxScope & VeloxScope::getRoot()
	{
		if (m_parent) {
			return m_parent->getRoot();
		}
		return *this;
	}

	size_t VeloxScope::getLevel()
	{
		if (m_parent) {
			return 1 + m_parent->getLevel();
		}
		return 0;
	}

	std::unique_ptr<VeloxScope> VeloxScope::createClosure()
	{
		auto result = VeloxScope::create();

		for (auto & it : m_items) {
			switch (it.second->getType()) {
			case VeloxItemType::INT:
			case VeloxItemType::REAL:
			case VeloxItemType::ARRAY:
			case VeloxItemType::OBJECT:
			case VeloxItemType::REFERENCE:
			case VeloxItemType::STRING:
				result->putItem(it.first, it.second->clone());
			}
		}

		if (isBlock() && hasParent()) {
			m_parent->injectItems(*result);
		}

		return result;
	}

	void VeloxScope::injectItems(VeloxScope & scope) const
	{
		for (auto & it : m_items) {
			switch (it.second->getType()) {
			case VeloxItemType::INT:
			case VeloxItemType::REAL:
			case VeloxItemType::ARRAY:
			case VeloxItemType::OBJECT:
			case VeloxItemType::REFERENCE:
			case VeloxItemType::STRING:
				scope.putItem(it.first, it.second->clone());
			}
		}

		if (isBlock() && hasParent()) {
			m_parent->injectItems(scope);
		}
	}

	bool VeloxScope::hasItem(const std::string & name) const
	{
		if (m_items.find(name) != m_items.end()) {
			return true;
		}

		return m_parent ? m_parent->hasItem(name) : false;
	}

	bool VeloxScope::hasLocalItem(const std::string & name) const
	{
		if (m_items.find(name) != m_items.end()) {
			return true;
		}

		if (isBlock() && m_parent) {
			return m_parent->hasLocalItem(name);
		}
		return false;
	}

	std::shared_ptr<VeloxItem> VeloxScope::getItem(const std::string & name)
	{
		auto result = findItem(name);
		if (!result) {
			throw std::logic_error("Unknown item '" + name + "'");
		}
		return result;
	}

	std::shared_ptr<const VeloxItem> VeloxScope::getItem(const std::string & name) const
	{
		auto result = findItem(name);
		if (!result) {
			throw std::logic_error("Unknown item '" + name + "'");
		}
		return result;
	}

	std::shared_ptr<VeloxItem> VeloxScope::findItem(const std::string & name)
	{
		auto it = m_items.find(name);
		if (it != m_items.end()) {
			return it->second;
		}

		if (!m_parent) {
			return nullptr;
		}

		return m_parent->findItem(name);
	}

	std::shared_ptr<const VeloxItem> VeloxScope::findItem(const std::string & name) const
	{
		auto it = m_items.find(name);
		if (it != m_items.end()) {
			return it->second;
		}

		if (!m_parent) {
			return nullptr;
		}

		return m_parent->findItem(name);
	}

	void VeloxScope::putItem(const std::string & name, std::shared_ptr<VeloxItem> item)
	{
		if (hasLocalItem(name)) {
			throw std::logic_error("Ambiguous item name '" + name + "'");
		}
		m_items[name] = item;
	}

	void VeloxScope::putItem(std::shared_ptr<VeloxItem> item)
	{
		m_anonymousItems.push_back(item);
	}

	void VeloxScope::clear()
	{
		m_items.clear();
		m_anonymousItems.clear();
	}

	int VeloxScope::getIntValue(const std::string & name) const
	{
		return getItem(name)->getIntValue();
	}

	bool VeloxScope::getBoolValue(const std::string & name) const
	{
		return getItem(name)->getBoolValue();
	}

	std::string VeloxScope::getStringValue(const std::string & name) const
	{
		return getItem(name)->getStringValue();
	}

	double VeloxScope::getDoubleValue(const std::string & name) const
	{
		return getItem(name)->getRealValue();
	}

	Real VeloxScope::getRealValue(const std::string & name) const
	{
		return static_cast<Real>(getItem(name)->getRealValue());
	}

	Real VeloxScope::getRealValue(const std::string & name, Real defaultValue) const
	{
		auto item = getItem(name);
		if (item->getType() != VeloxItemType::UNDEFINED) {
			return static_cast<Real>(item->getRealValue());
		}
		return defaultValue;
	}

	std::string VeloxScope::getStringValue(const std::string & name, const std::string & defaultValue) const
	{
		auto item = getItem(name);
		if (item->getType() != VeloxItemType::UNDEFINED) {
			return item->getStringValue();
		}
		return defaultValue;
	}

	Real VeloxScope::getRealValueWithinRange(const std::string & name, Real minValue, Real maxValue, Real defaultValue)
	{
		auto item = getItem(name);
		if (item->getType() == VeloxItemType::UNDEFINED) {
			return defaultValue;
		}

		Real result = static_cast<Real>(item->getRealValue());
		if (result < minValue || result > maxValue) {
			throw VeloxInterpreterException("'" + name + "' must be within interval [" 
				+ StringUtils::toString(minValue) + ", " + StringUtils::toString(maxValue) + "], got "
				+ StringUtils::toString(result));
		}
		return result;
	}

	int VeloxScope::getIntValue(const std::string & name, int defaultValue) const
	{
		auto item = getItem(name);
		if (item->getType() != VeloxItemType::UNDEFINED) {
			return item->getIntValue();
		}
		return defaultValue;
	}

	int VeloxScope::getIntLowerBounded(const std::string & name, int minValue, int defaultValue) const
	{
		auto item = getItem(name);
		if (item->getType() == VeloxItemType::UNDEFINED) {
			return defaultValue;
		}

		int result = item->getIntValue();
		if (result < minValue) {
			throw VeloxInterpreterException("'" + name + "' must be within greater or equal "
				+ std::to_string(minValue) + ", got " + std::to_string(result));
		}
		return result;
	}

	Real VeloxScope::getRealLowerBounded(const std::string & name, Real minValue, Real defaultValue) const
	{
		auto item = getItem(name);
		if (item->getType() == VeloxItemType::UNDEFINED) {
			return defaultValue;
		}

		Real result = static_cast<Real>(item->getRealValue());
		if (result < minValue) {
			throw VeloxInterpreterException("'" + name + "' must be within greater or equal "
				+ std::to_string(minValue) + ", got " + std::to_string(result));
		}
		return result;
	}

} // end of namespace

