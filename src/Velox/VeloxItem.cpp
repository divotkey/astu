// ___________________________________________________________________________
//
//                   Code Synthesized by SWORD. Do not modify!
//                           SWORD Interpreter v1.8.15
// ___________________________________________________________________________
// 

// Local includes
#include "Velox/VeloxItem.h"
#include "Velox/VeloxHook.h"
#include "Velox/VeloxInterpreter.h"
#include "Velox/VeloxScope.h"

// C++ Standard Library includes
#include <cassert>
#include <algorithm>
#include <new>
#include <cstdlib>
#include <cmath>

namespace astu {

		const std::vector<std::string> VeloxItem::TYPE_NAMES = {
			"UNDEFINED", 
			"INT", 
			"REAL", 
			"BOOL", 
			"STRING", 
			"FUNCTION", 
			"HOOK", 
			"CLASS", 
			"OBJECT", 
			"REFERENCE", 
			"ARRAY"
		};
	
	
	VeloxItem::VeloxItem(VeloxItemType type)
		: m_type(type), m_intData(0), m_realData(0), m_innerFunction(false)
	{
		PREPARE_POOLING();
	}

	const std::string & VeloxItem::toString(VeloxItemType type)
	{
		int idx = static_cast<int>(type);
		if (idx < 0 || idx >= TYPE_NAMES.size()) {
			throw std::logic_error("Unable to interpret node type (" + std::to_string(idx) + ")");
		}

		return TYPE_NAMES[idx];
	}

	std::shared_ptr<VeloxItem> VeloxItem::clone() const
	{
		switch (m_type) {
		case VeloxItemType::INT:
			return createIntValue(m_intData);

		case VeloxItemType::REAL:
			return createRealValue(m_realData);

		case VeloxItemType::BOOL:
			return createBoolValue(m_intData != 0);

		case VeloxItemType::STRING:
			return createStringValue(m_stringData);

		case VeloxItemType::REFERENCE:
			return createReference(m_reference);

		case VeloxItemType::FUNCTION:
			return createFunction(m_node);

		case VeloxItemType::HOOK:
			return createHook(m_hook);

		case VeloxItemType::OBJECT: {
			auto result = createObject(m_customData);
			for (auto & it : m_attributes) {
				result->addAttribute(it.first, it.second->clone());
			}
			return result;
		}

		case VeloxItemType::ARRAY: {
			auto result = createArray();
			for (auto & elem : m_arrayElems) {
				result->addArrayElem(elem->clone());
			}
			return result;
		}

		default:
			throw std::logic_error("item of type " + toString(m_type) + " cannot be copied");
		}
	}

	void VeloxItem::addAttribute(const std::string & name, std::shared_ptr<VeloxItem> attribute)
	{
		switch (m_type) {
		case VeloxItemType::REFERENCE:
			m_reference->addAttribute(name, attribute);
			break;

		case VeloxItemType::ARRAY:
		case VeloxItemType::OBJECT:
			if (hasAttribute(name)) {
				throw std::logic_error("ambiguous attribute name '" + name + "'");
			}
			m_attributes[name] = attribute;
			attribute->m_parent = shared_from_this();
			break;

		default:
			throw std::logic_error("Not an object nor a reference item");
		}
	}

	std::shared_ptr<VeloxItem> VeloxItem::getAttribute(const std::string & name) const
	{
		switch (m_type) {
		case VeloxItemType::REFERENCE:
			return m_reference->getAttribute(name);

		case VeloxItemType::ARRAY:
		case VeloxItemType::OBJECT: {
			auto it = m_attributes.find(name);
			if (it == m_attributes.end()) {
				throw std::logic_error("item attribute '" + name + "' not found");
			}
			return it->second;
		}

		default:
			throw VeloxInterpreterException("not an object, no field access to '" + name + "' not possible");
		}
	}

	bool VeloxItem::hasAttribute(const std::string & name) const
	{
		switch (m_type) {
		case VeloxItemType::REFERENCE:
			return m_reference->hasAttribute(name);

		case VeloxItemType::ARRAY:
		case VeloxItemType::OBJECT:
			return m_attributes.find(name) != m_attributes.end();

		default:
			return false;
		}
	}

	void VeloxItem::addArrayElem(std::shared_ptr<VeloxItem> elem)
	{
		switch (m_type) {
		case VeloxItemType::REFERENCE:
			return m_reference->addArrayElem(elem);

		case VeloxItemType::ARRAY:
			m_arrayElems.push_back(elem);
			getAttribute("length")->m_intData = static_cast<int>(m_arrayElems.size());
			break;

		default:
			throw VeloxInterpreterException("invalid type, array type expected");
		}
	}

	std::shared_ptr<VeloxItem> VeloxItem::getArrayElem(unsigned int index)
	{
		switch (m_type) {
		case VeloxItemType::REFERENCE:
			return m_reference->getArrayElem(index);

		case VeloxItemType::ARRAY:
			if (index < 0 || index >= m_arrayElems.size()) {
				throw VeloxInterpreterException("array index out of range, got index " + std::to_string(index));
			}
			return m_arrayElems[index];

		default:
			throw VeloxInterpreterException("invalid type, array type expected");
		}
	}

	const std::vector<std::shared_ptr<VeloxItem>> & VeloxItem::getArrayElems() const
	{
		switch (m_type) {
		case VeloxItemType::REFERENCE:
			return m_reference->getArrayElems();

		case VeloxItemType::ARRAY:
			return m_arrayElems;

		default:
			throw VeloxInterpreterException("invalid type, array type expected");
		}
	}

	size_t VeloxItem::numOfArrayElems() const
	{
		switch (m_type) {
		case VeloxItemType::REFERENCE:
			return m_reference->numOfArrayElems();

		case VeloxItemType::ARRAY:
			return m_arrayElems.size();

		default:
			throw std::logic_error("Item not of array type.");
		}
	}

	std::string VeloxItem::getStringValue(VeloxInterpreter & interpreter) const
	{
		switch (m_type) {
		case VeloxItemType::INT:
		case VeloxItemType::REAL:
		case VeloxItemType::BOOL:
		case VeloxItemType::STRING:
		case VeloxItemType::FUNCTION:
		case VeloxItemType::HOOK:
		case VeloxItemType::ARRAY:
		case VeloxItemType::UNDEFINED:
			return getStringValue();

		case VeloxItemType::REFERENCE:
			assert(m_reference);
			return m_reference->getStringValue(interpreter);

		case VeloxItemType::OBJECT: {
			auto it = m_attributes.find("toString");
			if (it != m_attributes.end()) {
				switch (it->second->getType()) {
				case VeloxItemType::HOOK:
				case VeloxItemType::FUNCTION:
					return interpreter.evaluate(*it->second)->getStringValue(interpreter);
				}
			}
			return "OBJ";
		}

		default:
			throw VeloxInterpreterException("internal interpreter error");
		}
	}

	void VeloxItem::setAttachable(std::shared_ptr<IAttachable> attachable)
	{
		if (m_type != VeloxItemType::OBJECT) {
			throw std::logic_error("Unable to set item type, item is not an object");
		}
		m_customData = attachable;
	}

	std::shared_ptr<IAttachable> VeloxItem::getAttachable() const {
		switch (m_type) {
		case VeloxItemType::REFERENCE:
			return m_reference->getAttachable();

		case VeloxItemType::OBJECT:
			return m_customData;

		default:
			throw std::logic_error("Not an object nor a reference item");
		}
	}

	std::shared_ptr<IAttachable> VeloxItem::findAttachable() const {
		switch (m_type) {
		case VeloxItemType::REFERENCE:
			return m_reference->findAttachable();

		case VeloxItemType::OBJECT:
			return m_customData;

		default:
			return nullptr;
		}
	}

	bool VeloxItem::hasAttachable() const {
		switch (m_type) {
		case VeloxItemType::REFERENCE:
			return m_reference->hasAttachable();

		case VeloxItemType::OBJECT:
			return true;

		default:
			return false;
		}
	}

	std::shared_ptr<VeloxItem> VeloxItem::operator++(int)
	{
		switch (m_type) {
		case VeloxItemType::INT:
			return createIntValue(m_intData++);

		case VeloxItemType::REAL:
			return createRealValue(m_realData++);

		default:
			throw VeloxInterpreterException(
				"postfix increment operator undefined for type "
				+ toString(m_type)
			);
		}
	}

	std::shared_ptr<VeloxItem> VeloxItem::operator--(int)
	{
		switch (m_type) {
		case VeloxItemType::INT:
			return createIntValue(m_intData--);

		case VeloxItemType::REAL:
			return createRealValue(m_realData--);

		default:
			throw VeloxInterpreterException(
				"postfix increment operator undefined for type "
					+ toString(m_type)
			);
		}
	}

	std::shared_ptr<VeloxItem> VeloxItem::operator!=(const VeloxItem & right)
	{
		auto result = *this == right;
		assert(result->getType() == VeloxItemType::BOOL);
		result->m_intData = !(result->m_intData != 0);
		return result;
	}

	std::string VeloxItem::getArrayElemsAsString() const
	{
		std::string result = "[";

		for (auto it = m_arrayElems.cbegin(); it != m_arrayElems.cend(); ) {
			result += (*it)->getStringValue();
			if (++it != m_arrayElems.cend()) {
				result += ", ";
			}
		}
		result += "]";
		return result;
	}

	std::shared_ptr<VeloxItem> VeloxItem::compareArrays(const VeloxItem & right)
	{
		assert(m_type == VeloxItemType::ARRAY);
		assert(right.m_type == VeloxItemType::ARRAY);

		if (m_arrayElems.size() != right.m_arrayElems.size()) {
			return VeloxItem::createBoolValue(false);
		}

		auto it1 = m_arrayElems.begin();
		auto it2 = right.m_arrayElems.begin();

		while (it1 != m_arrayElems.end()) {
			assert(it2 != right.m_arrayElems.end());

			std::shared_ptr<VeloxItem> result = **it1 == **it2;
			assert(result->m_type == VeloxItemType::BOOL);
			if (result->m_intData == 0) 
				return VeloxItem::createBoolValue(false);

			++it1;
			++it2;
		}

		return VeloxItem::createBoolValue(true);
	}

	void VeloxItem::setClosure(std::unique_ptr<VeloxScope> closure)
	{
		m_closure = std::move(closure);
	}

	VeloxScope & VeloxItem::getClosure() const
	{
		switch(m_type) {
		case VeloxItemType::REFERENCE:
			return m_reference->getClosure();
		default:
			if (!m_closure) {
				throw std::logic_error("No closure available");
			}
			return *m_closure;
		}
	}

	bool VeloxItem::hasClosure() const
	{
		switch(m_type) {
		case VeloxItemType::REFERENCE:
			return m_reference->hasClosure();
		default:
			return m_closure != nullptr;
		}
	}

	bool VeloxItem::hasParent() const
	{
		switch (m_type) {
		case VeloxItemType::REFERENCE:
			return m_reference->hasParent();

		default:
			auto parent = m_parent.lock();
			return parent != nullptr;
		}
	}

	std::shared_ptr<VeloxItem> VeloxItem::getParent() const
	{
		switch (m_type) {
		case VeloxItemType::REFERENCE:
			return m_reference->getParent();

		default:
			return m_parent.lock();
		}
	}

	bool VeloxItem::isInnerFunction() const
	{
		switch (m_type) {
		case VeloxItemType::REFERENCE:
			return m_reference->isInnerFunction();

		default:
			return m_innerFunction;
		}
	}

	int VeloxItem::getIntValue() const {
		switch(m_type) {
	
		case VeloxItemType::INT:
			return m_intData;
	
		case VeloxItemType::REAL:
			return static_cast<int>(m_realData);
	
		default:
			throw VeloxInterpreterException("type mismatch for type, int requested, actual type is " 
				+ VeloxItem::toString(m_type));
		}
	}

	double VeloxItem::getRealValue() const {
		switch(m_type) {
	
		case VeloxItemType::REAL:
			return m_realData;
	
		case VeloxItemType::INT:
			return static_cast<double>(m_intData);
	
		default:
			throw VeloxInterpreterException("type mismatch for type, double requested, actual type is " 
				+ VeloxItem::toString(m_type));
		}
	}

	bool VeloxItem::getBoolValue() const {
		switch(m_type) {
	
		case VeloxItemType::BOOL:
			return m_intData != 0;
	
		case VeloxItemType::INT:
			return m_intData != 0;
	
		case VeloxItemType::REAL:
			return m_realData != 0;
	
		default:
			throw VeloxInterpreterException("type mismatch for type, bool requested, actual type is " 
				+ VeloxItem::toString(m_type));
		}
	}

	std::string VeloxItem::getStringValue() const {
		switch(m_type) {
	
		case VeloxItemType::STRING:
			return m_stringData;
	
		case VeloxItemType::INT:
			return std::to_string(m_intData);
	
		case VeloxItemType::REAL:
			return std::to_string(m_realData);
	
		case VeloxItemType::UNDEFINED:
			return "UNDEFINED";
	
		case VeloxItemType::BOOL:
			return std::string(getBoolValue() ? "true" : "false");
	
		case VeloxItemType::FUNCTION:
			return "func:" + m_node->getStringData();
	
		case VeloxItemType::HOOK:
			return "hook:" + m_hook->getName();
	
		case VeloxItemType::ARRAY:
			return getArrayElemsAsString();
	
		case VeloxItemType::REFERENCE:
			return m_reference->getStringValue();
	
		default:
			throw VeloxInterpreterException("type mismatch for type, std::string requested, actual type is " 
				+ VeloxItem::toString(m_type));
		}
	}

	VeloxHook & VeloxItem::getHook() const {
		switch(m_type) {
	
		case VeloxItemType::HOOK:
			return *m_hook;
	
		default:
			throw VeloxInterpreterException("type mismatch for type, VeloxHook & requested, actual type is " 
				+ VeloxItem::toString(m_type));
		}
	}

	const VeloxNode & VeloxItem::getAstNode() const {
		switch(m_type) {
	
		case VeloxItemType::FUNCTION:
			return *m_node;
	
		case VeloxItemType::CLASS:
			return *m_node;
	
		case VeloxItemType::REFERENCE:
			return *m_reference->m_node;
	
		default:
			throw VeloxInterpreterException("type mismatch for type, const VeloxNode & requested, actual type is " 
				+ VeloxItem::toString(m_type));
		}
	}

	
	VeloxItem & VeloxItem::operator=(VeloxItem & right)
	{
		switch (m_type) {
			
			case VeloxItemType::UNDEFINED:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					// nothing to do
					break;
				
				case VeloxItemType::INT:
					m_type = VeloxItemType::INT; 
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::REAL:
					m_type = VeloxItemType::REAL; 
					m_realData = right.m_realData;
					break;
				
				case VeloxItemType::BOOL:
					m_type = VeloxItemType::BOOL; 
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::STRING:
					m_type = VeloxItemType::STRING; 
				m_stringData = right.m_stringData;
					break;
				
				case VeloxItemType::FUNCTION:
					m_type = VeloxItemType::REFERENCE; 
					m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::HOOK:
					m_type = VeloxItemType::HOOK; 
					m_hook = right.m_hook;
					break;
				
				case VeloxItemType::OBJECT:
					m_type = VeloxItemType::REFERENCE; 
					m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::REFERENCE:
					m_type = VeloxItemType::REFERENCE; 
			 m_reference = right.m_reference;
					break;
				
				case VeloxItemType::ARRAY:
					m_type = VeloxItemType::REFERENCE; 
			 m_reference = right.shared_from_this();
					break;
				
				default:
					throw VeloxInterpreterException(
						"undefined assignment for type UNDEFINED and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
				break;
			
			case VeloxItemType::INT:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					m_type = VeloxItemType::UNDEFINED;
					break;
				
				case VeloxItemType::INT:
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::REAL:
					m_type = VeloxItemType::REAL; 
					m_realData = right.m_realData;
					break;
				
				case VeloxItemType::BOOL:
					m_type = VeloxItemType::BOOL; 
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::STRING:
					m_type = VeloxItemType::STRING; 
				m_stringData = right.m_stringData;
					break;
				
				case VeloxItemType::FUNCTION:
					m_type = VeloxItemType::REFERENCE; 
					m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::HOOK:
					m_type = VeloxItemType::HOOK; 
					m_hook = right.m_hook;
					break;
				
				case VeloxItemType::OBJECT:
					m_type = VeloxItemType::REFERENCE; 
					m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::REFERENCE:
					m_type = VeloxItemType::REFERENCE; 
			 m_reference = right.m_reference;
					break;
				
				case VeloxItemType::ARRAY:
					m_type = VeloxItemType::REFERENCE; 
			 m_reference = right.shared_from_this();
					break;
				
				default:
					throw VeloxInterpreterException(
						"undefined assignment for type INT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
				break;
			
			case VeloxItemType::REAL:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					m_type = VeloxItemType::UNDEFINED;
					break;
				
				case VeloxItemType::INT:
					m_type = VeloxItemType::INT; 
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::REAL:
					m_realData = right.m_realData;
					break;
				
				case VeloxItemType::BOOL:
					m_type = VeloxItemType::BOOL; 
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::STRING:
					m_type = VeloxItemType::STRING; 
				m_stringData = right.m_stringData;
					break;
				
				case VeloxItemType::FUNCTION:
					m_type = VeloxItemType::REFERENCE; 
					m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::HOOK:
					m_type = VeloxItemType::HOOK; 
					m_hook = right.m_hook;
					break;
				
				case VeloxItemType::OBJECT:
					m_type = VeloxItemType::REFERENCE; 
					m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::REFERENCE:
					m_type = VeloxItemType::REFERENCE; m_reference = right.m_reference;
					break;
				
				case VeloxItemType::ARRAY:
					m_type = VeloxItemType::REFERENCE; 
			 m_reference = right.shared_from_this();
					break;
				
				default:
					throw VeloxInterpreterException(
						"undefined assignment for type REAL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
				break;
			
			case VeloxItemType::BOOL:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					m_type = VeloxItemType::UNDEFINED;
					break;
				
				case VeloxItemType::INT:
					m_type = VeloxItemType::INT; 
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::REAL:
					m_type = VeloxItemType::REAL; 
					m_realData = right.m_realData;
					break;
				
				case VeloxItemType::BOOL:
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::STRING:
					m_type = VeloxItemType::STRING; 
				m_stringData = right.m_stringData;
					break;
				
				case VeloxItemType::FUNCTION:
					m_type = VeloxItemType::REFERENCE; 
					m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::HOOK:
					m_type = VeloxItemType::HOOK; 
					m_hook = right.m_hook;
					break;
				
				case VeloxItemType::OBJECT:
					m_type = VeloxItemType::REFERENCE; 
					m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::REFERENCE:
					m_type = VeloxItemType::REFERENCE; 
			 m_reference = right.m_reference;
					break;
				
				case VeloxItemType::ARRAY:
					m_type = VeloxItemType::REFERENCE; 
			 m_reference = right.shared_from_this();
					break;
				
				default:
					throw VeloxInterpreterException(
						"undefined assignment for type BOOL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
				break;
			
			case VeloxItemType::STRING:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					m_type = VeloxItemType::UNDEFINED; 
					m_stringData.clear();
					break;
				
				case VeloxItemType::INT:
					m_type = VeloxItemType::INT; 
					m_stringData.clear(); 
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::REAL:
					m_type = VeloxItemType::REAL; 
					m_stringData.clear(); 
					m_realData = right.m_realData;
					break;
				
				case VeloxItemType::BOOL:
					m_type = VeloxItemType::BOOL; 
					m_stringData.clear(); 
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::STRING:
					m_type = VeloxItemType::STRING; 
				m_stringData = right.m_stringData;
					break;
				
				case VeloxItemType::FUNCTION:
					m_type = VeloxItemType::REFERENCE; 
					m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::HOOK:
					m_type = VeloxItemType::FUNCTION; 
					m_stringData.clear(); 
					m_hook = right.m_hook;
					break;
				
				case VeloxItemType::OBJECT:
					m_type = VeloxItemType::REFERENCE; 
					m_stringData.clear(); 
					m_reference = right.m_reference;
					break;
				
				case VeloxItemType::REFERENCE:
					m_type = VeloxItemType::FUNCTION; 
					m_stringData.clear(); 
					m_reference = right.m_reference;
					break;
				
				case VeloxItemType::ARRAY:
					m_type = VeloxItemType::REFERENCE; 
			 m_reference = right.shared_from_this();
					break;
				
				default:
					throw VeloxInterpreterException(
						"undefined assignment for type STRING and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
				break;
			
			case VeloxItemType::FUNCTION:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					m_type = VeloxItemType::UNDEFINED; 
					m_node = nullptr;
					break;
				
				case VeloxItemType::INT:
					m_type = VeloxItemType::INT; 
					m_node = nullptr; 
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::REAL:
					m_type = VeloxItemType::REAL; 
					m_node = nullptr; 
					m_realData = right.m_realData;
					break;
				
				case VeloxItemType::BOOL:
					m_type = VeloxItemType::BOOL; 
					m_node = nullptr; 
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::STRING:
					m_type = VeloxItemType::STRING; 
					m_node = nullptr; 
				m_stringData = right.m_stringData;
					break;
				
				case VeloxItemType::FUNCTION:
					m_type = VeloxItemType::REFERENCE; 
					m_node = nullptr; 
					m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::HOOK:
					m_type = VeloxItemType::HOOK; 
					m_node = nullptr; 
					m_hook = right.m_hook;
					break;
				
				case VeloxItemType::OBJECT:
					m_type = VeloxItemType::REFERENCE; 
				m_node = nullptr; 
					m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::REFERENCE:
					m_type = VeloxItemType::REFERENCE; 
					m_node = nullptr; 
			 m_reference = right.m_reference;
					break;
				
				case VeloxItemType::ARRAY:
					m_type = VeloxItemType::REFERENCE; 
					m_node = nullptr; 
			 m_reference = right.shared_from_this();
					break;
				
				default:
					throw VeloxInterpreterException(
						"undefined assignment for type FUNCTION and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
				break;
			
			case VeloxItemType::HOOK:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					m_type = VeloxItemType::UNDEFINED; 
					m_hook = nullptr;
					break;
				
				case VeloxItemType::INT:
					m_type = VeloxItemType::INT; 
					m_hook = nullptr; 
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::REAL:
					m_type = VeloxItemType::REAL; 
					m_hook = nullptr; 
					m_realData = right.m_realData;
					break;
				
				case VeloxItemType::BOOL:
					m_type = VeloxItemType::BOOL; 
					m_hook = nullptr; 
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::STRING:
					m_type = VeloxItemType::STRING; 
					m_hook = nullptr; 
				m_stringData = right.m_stringData;
					break;
				
				case VeloxItemType::FUNCTION:
					m_type = VeloxItemType::REFERENCE;		m_hook = nullptr; 
					m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::HOOK:
					m_hook = right.m_hook;
					break;
				
				case VeloxItemType::OBJECT:
					m_type = VeloxItemType::REFERENCE; 
					m_hook = nullptr; 
					m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::REFERENCE:
					m_type = VeloxItemType::REFERENCE; 
					m_hook = nullptr; 
			 m_reference = right.m_reference;
					break;
				
				case VeloxItemType::ARRAY:
					m_type = VeloxItemType::REFERENCE; 
					m_hook = nullptr; 
			 m_reference = right.shared_from_this();
					break;
				
				default:
					throw VeloxInterpreterException(
						"undefined assignment for type HOOK and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
				break;
			
			case VeloxItemType::OBJECT:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					m_type = VeloxItemType::UNDEFINED; 
				m_customData = nullptr;
					break;
				
				case VeloxItemType::INT:
					m_type = VeloxItemType::INT; 
					m_customData = nullptr; 
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::REAL:
					m_type = VeloxItemType::REAL; 
					m_customData = nullptr; 
					m_realData = right.m_realData;
					break;
				
				case VeloxItemType::BOOL:
					m_type = VeloxItemType::BOOL; 
					m_customData = nullptr; 
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::STRING:
					m_type = VeloxItemType::STRING; 
					m_customData = nullptr; 
				m_stringData = right.m_stringData;
					break;
				
				case VeloxItemType::FUNCTION:
					m_type = VeloxItemType::REFERENCE;		m_customData = nullptr; 
					m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::HOOK:
					m_type = VeloxItemType::HOOK; 
				m_customData = nullptr; 
				m_hook = right.m_hook;
					break;
				
				case VeloxItemType::OBJECT:
					m_type = VeloxItemType::REFERENCE; 
					m_customData = nullptr; 
				m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::REFERENCE:
					m_type = VeloxItemType::REFERENCE; 
				m_customData = nullptr; 
				m_reference = right.m_reference;
					break;
				
				case VeloxItemType::ARRAY:
					m_type = VeloxItemType::REFERENCE; 
					m_customData = nullptr; 
			 m_reference = right.shared_from_this();
					break;
				
				default:
					throw VeloxInterpreterException(
						"undefined assignment for type OBJECT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
				break;
			
			case VeloxItemType::REFERENCE:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					m_type = VeloxItemType::UNDEFINED; 
				m_reference = nullptr; 
					m_node = nullptr;
					break;
				
				case VeloxItemType::INT:
					m_type = VeloxItemType::INT; 
				m_reference = nullptr; 
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::REAL:
					m_type = VeloxItemType::REAL; 
				m_reference = nullptr; 
					m_realData = right.m_realData;
					break;
				
				case VeloxItemType::BOOL:
					m_type = VeloxItemType::BOOL; 
				m_reference = nullptr; 
					m_intData = right.m_intData;
					break;
				
				case VeloxItemType::STRING:
					m_type = VeloxItemType::STRING; 
					m_reference = nullptr; 
				m_stringData = right.m_stringData;
					break;
				
				case VeloxItemType::FUNCTION:
					m_type = VeloxItemType::REFERENCE; 
					m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::HOOK:
					m_type = VeloxItemType::HOOK; 
				m_reference = nullptr; 
				m_hook = right.m_hook;
					break;
				
				case VeloxItemType::OBJECT:
					m_reference = right.shared_from_this();
					break;
				
				case VeloxItemType::REFERENCE:
					m_reference = right.m_reference;
					break;
				
				case VeloxItemType::ARRAY:
					m_type = VeloxItemType::REFERENCE; 
			 m_reference = right.shared_from_this();
					break;
				
				default:
					throw VeloxInterpreterException(
						"undefined assignment for type REFERENCE and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
				break;
			
			default:
				throw VeloxInterpreterException(
					"undefined assignment for type " + VeloxItem::toString(m_type) + " and " 
					+ VeloxItem::toString(right.m_type)
				);
			}
	
			return *this;
		}
		std::shared_ptr<VeloxItem> VeloxItem::operator==(const VeloxItem & right)
		{
			switch (m_type) {
			
			case VeloxItemType::UNDEFINED:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					return VeloxItem::createBoolValue(true);
				
				case VeloxItemType::INT:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::REAL:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::BOOL:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::FUNCTION:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::HOOK:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::ARRAY:
					return VeloxItem::createBoolValue(false);
				
				default:
					throw VeloxInterpreterException(
						"operator '==' undefined for type UNDEFINED and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::INT:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::INT:
					return VeloxItem::createBoolValue(m_intData == right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createBoolValue(m_intData == right.m_realData);
				
				default:
					throw VeloxInterpreterException(
						"operator '==' undefined for type INT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REAL:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::INT:
					return VeloxItem::createBoolValue(m_realData == right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createBoolValue(m_realData == right.m_realData);
				
				default:
					throw VeloxInterpreterException(
						"operator '==' undefined for type REAL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::BOOL:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::BOOL:
					return VeloxItem::createBoolValue(getBoolValue() == right.getBoolValue());
				
				default:
					throw VeloxInterpreterException(
						"operator '==' undefined for type BOOL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::STRING:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::INT:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::REAL:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::BOOL:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::STRING:
					return VeloxItem::createBoolValue(m_stringData == right.m_stringData);
				
				case VeloxItemType::FUNCTION:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::HOOK:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::OBJECT:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::REFERENCE:
					return *this == *right.m_reference;
				
				case VeloxItemType::ARRAY:
					return VeloxItem::createBoolValue(false);
				
				default:
					throw VeloxInterpreterException(
						"operator '==' undefined for type STRING and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::FUNCTION:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::FUNCTION:
					return VeloxItem::createBoolValue(m_node == right.m_node);
				
				default:
					throw VeloxInterpreterException(
						"operator '==' undefined for type FUNCTION and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::HOOK:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::HOOK:
					return VeloxItem::createBoolValue(m_hook == right.m_hook);
				
				default:
					throw VeloxInterpreterException(
						"operator '==' undefined for type HOOK and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::OBJECT:
				switch(right.m_type) {
				
				case VeloxItemType::OBJECT:
					return VeloxItem::createBoolValue(this == &right);
				
				default:
					throw VeloxInterpreterException(
						"operator '==' undefined for type OBJECT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REFERENCE:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::REFERENCE:
					return *m_reference == *right.m_reference;
				
				default:
					throw VeloxInterpreterException(
						"operator '==' undefined for type REFERENCE and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::ARRAY:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					return VeloxItem::createBoolValue(false);
				
				case VeloxItemType::ARRAY:
					return compareArrays(right);
				
				default:
					throw VeloxInterpreterException(
						"operator '==' undefined for type ARRAY and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '==' undefined for type " + VeloxItem::toString(m_type) + " and " 
					+ VeloxItem::toString(right.m_type)
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator+() const
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				return VeloxItem::createIntValue(m_intData);
			
			case VeloxItemType::REAL:
				return VeloxItem::createRealValue(m_realData);
			
			default:
				throw VeloxInterpreterException(
					"unary operator '+' undefined for type " + VeloxItem::toString(m_type) 
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator-() const
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				return VeloxItem::createIntValue(-m_intData);
			
			case VeloxItemType::REAL:
				return VeloxItem::createRealValue(-m_realData);
			
			default:
				throw VeloxInterpreterException(
					"unary operator '-' undefined for type " + VeloxItem::toString(m_type) 
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator!() const
		{
			switch (m_type) {
			
			case VeloxItemType::BOOL:
				return VeloxItem::createBoolValue(m_intData == 0);
			
			default:
				throw VeloxInterpreterException(
					"unary operator '!' undefined for type " + VeloxItem::toString(m_type) 
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator++() 
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				++m_intData;		
				return shared_from_this();
			
			case VeloxItemType::REAL:
				++m_realData;		
				return shared_from_this();
			
			default:
				throw VeloxInterpreterException(
					"unary operator '++' undefined for type " + VeloxItem::toString(m_type) 
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator--() 
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				--m_intData;		
				return shared_from_this();
			
			case VeloxItemType::REAL:
				--m_realData;		
				return shared_from_this();
			
			default:
				throw VeloxInterpreterException(
					"unary operator '--' undefined for type " + VeloxItem::toString(m_type) 
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator+(VeloxItem & right) const
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return VeloxItem::createIntValue(m_intData + right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createRealValue(m_intData + right.m_realData);
				
				case VeloxItemType::STRING:
					return VeloxItem::createStringValue(getStringValue() + right.m_stringData);
							
				default:
					throw VeloxInterpreterException(
						"operator '+' undefined for type INT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REAL:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return VeloxItem::createRealValue(m_realData + right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createRealValue(m_realData + right.m_realData);
				
				case VeloxItemType::STRING:
					return VeloxItem::createStringValue(getStringValue() + right.m_stringData);
							
				default:
					throw VeloxInterpreterException(
						"operator '+' undefined for type REAL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::BOOL:
				switch(right.m_type) {
				
				case VeloxItemType::STRING:
					return VeloxItem::createStringValue(getStringValue() + right.m_stringData);
							
				default:
					throw VeloxInterpreterException(
						"operator '+' undefined for type BOOL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::STRING:
				switch(right.m_type) {
				
				case VeloxItemType::UNDEFINED:
					return VeloxItem::createStringValue(m_stringData + right.getStringValue());
				
				case VeloxItemType::INT:
					return VeloxItem::createStringValue(m_stringData + right.getStringValue());
				
				case VeloxItemType::REAL:
					return VeloxItem::createStringValue(m_stringData + right.getStringValue());
				
				case VeloxItemType::BOOL:
					return VeloxItem::createStringValue(m_stringData + right.getStringValue());
				
				case VeloxItemType::STRING:
					return VeloxItem::createStringValue(m_stringData + right.m_stringData);
				
				case VeloxItemType::FUNCTION:
					return VeloxItem::createStringValue(m_stringData + right.getStringValue());
				
				case VeloxItemType::REFERENCE:
					return VeloxItem::createStringValue(m_stringData + right.getStringValue());
				
				case VeloxItemType::ARRAY:
					return VeloxItem::createStringValue(m_stringData + right.getStringValue());
							
				default:
					throw VeloxInterpreterException(
						"operator '+' undefined for type STRING and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REFERENCE:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return *m_reference + right;
				
				case VeloxItemType::REFERENCE:
					return *m_reference + right;
							
				default:
					throw VeloxInterpreterException(
						"operator '+' undefined for type REFERENCE and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::ARRAY:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
								{ 
				auto result = clone(); 
				 result->addArrayElem(right.clone()); 
					return result; 
				 }
				
				case VeloxItemType::REAL:
								{ 
				auto result = clone(); 
				 result->addArrayElem(right.clone()); 
					return result; 
				 }
				
				case VeloxItemType::BOOL:
								{ 
				auto result = clone(); 
				 result->addArrayElem(right.clone()); 
					return result; 
				 }
				
				case VeloxItemType::STRING:
								{ 
				auto result = clone(); 
				 result->addArrayElem(right.clone()); 
					return result; 
				 }
				
				case VeloxItemType::FUNCTION:
								{ 
				auto result = clone(); 
				 result->addArrayElem(right.clone()); 
					return result; 
				 }
				
				case VeloxItemType::HOOK:
								{ 
				auto result = clone(); 
				 result->addArrayElem(right.clone()); 
					return result; 
				 }
				
				case VeloxItemType::REFERENCE:
								{ 
				auto result = clone(); 
				 result->addArrayElem(right.clone()); 
					return result; 
				 }
							
				default:
					throw VeloxInterpreterException(
						"operator '+' undefined for type ARRAY and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '+' undefined for type " + VeloxItem::toString(m_type) 
					+ " and " + VeloxItem::toString(right.m_type)
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator-(VeloxItem & right) const
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return VeloxItem::createIntValue(m_intData - right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createRealValue(m_intData - right.m_realData);
							
				default:
					throw VeloxInterpreterException(
						"operator '-' undefined for type INT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REAL:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return VeloxItem::createRealValue(m_realData - right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createRealValue(m_realData - right.m_realData);
							
				default:
					throw VeloxInterpreterException(
						"operator '-' undefined for type REAL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '-' undefined for type " + VeloxItem::toString(m_type) 
					+ " and " + VeloxItem::toString(right.m_type)
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator*(VeloxItem & right) const
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return VeloxItem::createIntValue(m_intData * right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createRealValue(m_intData * right.m_realData);
							
				default:
					throw VeloxInterpreterException(
						"operator '*' undefined for type INT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REAL:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return VeloxItem::createRealValue(m_realData * right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createRealValue(m_realData * right.m_realData);
							
				default:
					throw VeloxInterpreterException(
						"operator '*' undefined for type REAL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '*' undefined for type " + VeloxItem::toString(m_type) 
					+ " and " + VeloxItem::toString(right.m_type)
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator/(VeloxItem & right) const
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					if (right.m_intData == 0)
						throw VeloxInterpreterException("division by zero");
					return VeloxItem::createIntValue(m_intData / right.m_intData);
				
				case VeloxItemType::REAL:
					if (right.m_realData == 0)
						throw VeloxInterpreterException("division by zero");
					return VeloxItem::createRealValue(m_intData / right.m_realData);
							
				default:
					throw VeloxInterpreterException(
						"operator '/' undefined for type INT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REAL:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					if (right.m_intData == 0)
						throw VeloxInterpreterException("division by zero");
					return VeloxItem::createRealValue(m_realData / right.m_intData);
				
				case VeloxItemType::REAL:
					if (right.m_realData == 0)
						throw VeloxInterpreterException("division by zero");
					return VeloxItem::createRealValue(m_realData / right.m_realData);
							
				default:
					throw VeloxInterpreterException(
						"operator '/' undefined for type REAL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '/' undefined for type " + VeloxItem::toString(m_type) 
					+ " and " + VeloxItem::toString(right.m_type)
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator%(VeloxItem & right) const
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return VeloxItem::createIntValue(m_intData % right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createRealValue(fmod(m_intData, right.m_realData));
							
				default:
					throw VeloxInterpreterException(
						"operator '%' undefined for type INT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REAL:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return VeloxItem::createRealValue(fmod(m_realData, right.m_intData));
				
				case VeloxItemType::REAL:
					return VeloxItem::createRealValue(fmod(m_realData, right.m_realData));
							
				default:
					throw VeloxInterpreterException(
						"operator '%' undefined for type REAL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '%' undefined for type " + VeloxItem::toString(m_type) 
					+ " and " + VeloxItem::toString(right.m_type)
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator<(VeloxItem & right) const
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return VeloxItem::createBoolValue(m_intData < right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createBoolValue(m_intData < right.m_realData);
							
				default:
					throw VeloxInterpreterException(
						"operator '<' undefined for type INT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REAL:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return VeloxItem::createBoolValue(m_realData < right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createBoolValue(m_realData < right.m_realData);
							
				default:
					throw VeloxInterpreterException(
						"operator '<' undefined for type REAL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '<' undefined for type " + VeloxItem::toString(m_type) 
					+ " and " + VeloxItem::toString(right.m_type)
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator<=(VeloxItem & right) const
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return VeloxItem::createBoolValue(m_intData <= right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createBoolValue(m_intData <= right.m_realData);
							
				default:
					throw VeloxInterpreterException(
						"operator '<=' undefined for type INT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REAL:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return VeloxItem::createBoolValue(m_realData <= right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createBoolValue(m_realData <= right.m_realData);
							
				default:
					throw VeloxInterpreterException(
						"operator '<=' undefined for type REAL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '<=' undefined for type " + VeloxItem::toString(m_type) 
					+ " and " + VeloxItem::toString(right.m_type)
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator>(VeloxItem & right) const
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return VeloxItem::createBoolValue(m_intData > right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createBoolValue(m_intData > right.m_realData);
							
				default:
					throw VeloxInterpreterException(
						"operator '>' undefined for type INT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REAL:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return VeloxItem::createBoolValue(m_realData > right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createBoolValue(m_realData > right.m_realData);
							
				default:
					throw VeloxInterpreterException(
						"operator '>' undefined for type REAL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '>' undefined for type " + VeloxItem::toString(m_type) 
					+ " and " + VeloxItem::toString(right.m_type)
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator>=(VeloxItem & right) const
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return VeloxItem::createBoolValue(m_intData >= right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createBoolValue(m_intData >= right.m_realData);
							
				default:
					throw VeloxInterpreterException(
						"operator '>=' undefined for type INT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REAL:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return VeloxItem::createBoolValue(m_realData >= right.m_intData);
				
				case VeloxItemType::REAL:
					return VeloxItem::createBoolValue(m_realData >= right.m_realData);
							
				default:
					throw VeloxInterpreterException(
						"operator '>=' undefined for type REAL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '>=' undefined for type " + VeloxItem::toString(m_type) 
					+ " and " + VeloxItem::toString(right.m_type)
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator&&(VeloxItem & right) const
		{
			switch (m_type) {
			
			case VeloxItemType::BOOL:
				switch(right.m_type) {
				
				case VeloxItemType::BOOL:
					return VeloxItem::createBoolValue(m_intData && right.m_intData);
							
				default:
					throw VeloxInterpreterException(
						"operator '&&' undefined for type BOOL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '&&' undefined for type " + VeloxItem::toString(m_type) 
					+ " and " + VeloxItem::toString(right.m_type)
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator||(VeloxItem & right) const
		{
			switch (m_type) {
			
			case VeloxItemType::BOOL:
				switch(right.m_type) {
				
				case VeloxItemType::BOOL:
					return VeloxItem::createBoolValue(m_intData || right.m_intData);
							
				default:
					throw VeloxInterpreterException(
						"operator '||' undefined for type BOOL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '||' undefined for type " + VeloxItem::toString(m_type) 
					+ " and " + VeloxItem::toString(right.m_type)
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator+=(VeloxItem & right) 
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					m_intData += right.m_intData;		
					return shared_from_this();
				
				case VeloxItemType::REAL:
					m_intData += static_cast<int>(right.m_realData);		
					return shared_from_this();
							
				default:
					throw VeloxInterpreterException(
						"operator '+=' undefined for type INT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REAL:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					m_realData += right.m_intData;		
					return shared_from_this();
				
				case VeloxItemType::REAL:
					m_realData += right.m_realData;		
					return shared_from_this();
							
				default:
					throw VeloxInterpreterException(
						"operator '+=' undefined for type REAL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::STRING:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					m_stringData += right.getStringValue();		
					return shared_from_this();
				
				case VeloxItemType::REAL:
					m_stringData += right.getStringValue();		
					return shared_from_this();
				
				case VeloxItemType::BOOL:
					m_stringData += right.getStringValue();		
					return shared_from_this();
				
				case VeloxItemType::STRING:
					m_stringData += right.m_stringData;		
					return shared_from_this();
							
				default:
					throw VeloxInterpreterException(
						"operator '+=' undefined for type STRING and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REFERENCE:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					return *m_reference += right;
				
				case VeloxItemType::REAL:
					return *m_reference += right;
				
				case VeloxItemType::BOOL:
					return *m_reference += right;
				
				case VeloxItemType::STRING:
					return *m_reference += right;
				
				case VeloxItemType::FUNCTION:
					return *m_reference += right;
				
				case VeloxItemType::HOOK:
					return *m_reference += right;
				
				case VeloxItemType::OBJECT:
					return *m_reference += right;
				
				case VeloxItemType::REFERENCE:
					return *m_reference += right;
				
				case VeloxItemType::ARRAY:
					return *m_reference += right;
							
				default:
					throw VeloxInterpreterException(
						"operator '+=' undefined for type REFERENCE and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::ARRAY:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					addArrayElem(right.clone()); 
					return shared_from_this();
				
				case VeloxItemType::REAL:
					addArrayElem(right.clone()); 
					return shared_from_this();
				
				case VeloxItemType::BOOL:
					addArrayElem(right.clone()); 
					return shared_from_this();
				
				case VeloxItemType::STRING:
					addArrayElem(right.clone()); 
					return shared_from_this();
				
				case VeloxItemType::FUNCTION:
					addArrayElem(right.clone()); 
					return shared_from_this();
				
				case VeloxItemType::HOOK:
					addArrayElem(right.clone()); 
					return shared_from_this();
				
				case VeloxItemType::OBJECT:
					addArrayElem(createReference(right.shared_from_this())); 
					return shared_from_this();
				
				case VeloxItemType::REFERENCE:
					addArrayElem(right.clone()); 
					return shared_from_this();
				
				case VeloxItemType::ARRAY:
					addArrayElem(right.clone()); 
					return shared_from_this();
							
				default:
					throw VeloxInterpreterException(
						"operator '+=' undefined for type ARRAY and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '+=' undefined for type " + VeloxItem::toString(m_type) 
					+ " and " + VeloxItem::toString(right.m_type)
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator-=(VeloxItem & right) 
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					m_intData -= right.m_intData;		
					return shared_from_this();
				
				case VeloxItemType::REAL:
					m_intData -= static_cast<int>(right.m_realData);		
					return shared_from_this();
							
				default:
					throw VeloxInterpreterException(
						"operator '-=' undefined for type INT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REAL:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					m_realData -= right.m_intData;		
					return shared_from_this();
				
				case VeloxItemType::REAL:
					m_realData -= right.m_realData;		
					return shared_from_this();
							
				default:
					throw VeloxInterpreterException(
						"operator '-=' undefined for type REAL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '-=' undefined for type " + VeloxItem::toString(m_type) 
					+ " and " + VeloxItem::toString(right.m_type)
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator/=(VeloxItem & right) 
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					if (right.m_intData == 0)
						throw VeloxInterpreterException("division by zero");
					m_intData /= right.m_intData;		
					return shared_from_this();
				
				case VeloxItemType::REAL:
					if (right.m_realData == 0)
						throw VeloxInterpreterException("division by zero");
					m_intData /= static_cast<int>(right.m_realData);		
					return shared_from_this();
							
				default:
					throw VeloxInterpreterException(
						"operator '/=' undefined for type INT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REAL:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					if (right.m_intData == 0)
						throw VeloxInterpreterException("division by zero");
					m_realData /= right.m_intData;		
					return shared_from_this();
				
				case VeloxItemType::REAL:
					if (right.m_realData == 0)
						throw VeloxInterpreterException("division by zero");
					m_realData /= right.m_realData;		
					return shared_from_this();
							
				default:
					throw VeloxInterpreterException(
						"operator '/=' undefined for type REAL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '/=' undefined for type " + VeloxItem::toString(m_type) 
					+ " and " + VeloxItem::toString(right.m_type)
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator*=(VeloxItem & right) 
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					m_intData *= right.m_intData;		
					return shared_from_this();
				
				case VeloxItemType::REAL:
					m_intData *= static_cast<int>(right.m_realData);		
					return shared_from_this();
							
				default:
					throw VeloxInterpreterException(
						"operator '*=' undefined for type INT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REAL:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					m_realData *= right.m_intData;		
					return shared_from_this();
				
				case VeloxItemType::REAL:
					m_realData *= right.m_realData;		
					return shared_from_this();
							
				default:
					throw VeloxInterpreterException(
						"operator '*=' undefined for type REAL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '*=' undefined for type " + VeloxItem::toString(m_type) 
					+ " and " + VeloxItem::toString(right.m_type)
				);
			}
		}
	
		std::shared_ptr<VeloxItem> VeloxItem::operator%=(VeloxItem & right) 
		{
			switch (m_type) {
			
			case VeloxItemType::INT:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					m_intData &= right.m_intData;		
					return shared_from_this();
				
				case VeloxItemType::REAL:
					m_intData %= static_cast<int>(right.m_realData);		
					return shared_from_this();
							
				default:
					throw VeloxInterpreterException(
						"operator '%=' undefined for type INT and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			case VeloxItemType::REAL:
				switch(right.m_type) {
				
				case VeloxItemType::INT:
					m_realData = fmod(m_realData, right.m_intData);		
					return shared_from_this();
				
				case VeloxItemType::REAL:
					m_realData = fmod(m_realData, right.m_realData);		
					return shared_from_this();
							
				default:
					throw VeloxInterpreterException(
						"operator '%=' undefined for type REAL and " 
						+ VeloxItem::toString(right.m_type)
					);
				}
			
			default:
				throw VeloxInterpreterException(
					"operator '%=' undefined for type " + VeloxItem::toString(m_type) 
					+ " and " + VeloxItem::toString(right.m_type)
				);
			}
		}
	
} // end of namespace