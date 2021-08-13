// ___________________________________________________________________________
//
//                   Code Synthesized by SWORD. Do not modify!
//                           SWORD Interpreter v1.8.15
// ___________________________________________________________________________
// 

// Local includes
#include "Velox/VeloxTypes.h"
#include "Velox/IAttachable.h"
#include "Velox/VeloxInterpreterException.h"
#include "Util/Pooled.h"

// C++ Standard Library includes
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <vector>

#pragma once

namespace astu {

	// Forward declaration.
	class VeloxNode;
	class VeloxHook;
	class VeloxInterpreter;
	class VeloxScope;

		enum class VeloxItemType {
			UNDEFINED, 
			INT, 
			REAL, 
			BOOL, 
			STRING, 
			FUNCTION, 
			HOOK, 
			CLASS, 
			OBJECT, 
			REFERENCE, 
			ARRAY
		};
	
		class VeloxItem : public Pooled<VeloxItem>, public std::enable_shared_from_this<VeloxItem> {
		public:
	
			/**
			 * Returns a string representation of the specified item type.
			 *
			 * @param type	the item type
			 * @return the string representation of the specified item type
			 */
			static const std::string & toString(VeloxItemType type);
		
			static std::shared_ptr<VeloxItem> createUndefined() {
				return std::shared_ptr<VeloxItem>(new VeloxItem(VeloxItemType::UNDEFINED));
			}
		
			static std::shared_ptr<VeloxItem> createStringValue(const std::string & value) {
				auto result = std::shared_ptr<VeloxItem>(new VeloxItem(VeloxItemType::STRING));
				result->m_stringData = value;
				return result;
			}
		
			static std::shared_ptr<VeloxItem> createIntValue(int value) {
				auto result = std::shared_ptr<VeloxItem>(new VeloxItem(VeloxItemType::INT));
				result->m_intData = value;
				return result;
			}
		
			static std::shared_ptr<VeloxItem> createRealValue(double value) {
				auto result = std::shared_ptr<VeloxItem>(new VeloxItem(VeloxItemType::REAL));
				result->m_realData = value;
				return result;
			}
		
			static std::shared_ptr<VeloxItem> createBoolValue(bool value) {
				auto result = std::shared_ptr<VeloxItem>(new VeloxItem(VeloxItemType::BOOL));
				result->m_intData = value;
				return result;
			}
		
			static std::shared_ptr<VeloxItem> createFunction(std::shared_ptr<const VeloxNode> node, bool innerFunction = false) {
				auto result = std::shared_ptr<VeloxItem>(new VeloxItem(VeloxItemType::FUNCTION));
				result->m_node = node;
				result->m_innerFunction = innerFunction;
				return result;
			}
		
			static std::shared_ptr<VeloxItem> createClass(std::shared_ptr<const VeloxNode> node) {
				auto result = std::shared_ptr<VeloxItem>(new VeloxItem(VeloxItemType::CLASS));
				result->m_node = node;
				return result;
			}
		
			static std::shared_ptr<VeloxItem> createObject(std::shared_ptr<IAttachable> data = nullptr) {
				auto result = std::shared_ptr<VeloxItem>(new VeloxItem(VeloxItemType::OBJECT));
				result->m_customData = data;
				return result;
			}
		
			static std::shared_ptr<VeloxItem> createReference(std::shared_ptr<VeloxItem> reference) {
				auto result =  std::shared_ptr<VeloxItem>(new VeloxItem(VeloxItemType::REFERENCE));
				result->m_reference = reference;
				return result;
			}
		
			static std::shared_ptr<VeloxItem> createHook(std::shared_ptr<VeloxHook> hook) {
				auto result = std::shared_ptr<VeloxItem>(new VeloxItem(VeloxItemType::HOOK));
				result->m_hook = hook;
				return result;
			}
		
			static std::shared_ptr<VeloxItem> createArray() {
				auto result = std::shared_ptr<VeloxItem>(new VeloxItem(VeloxItemType::ARRAY));
				result->addAttribute("length", createIntValue(0));
				return result;
			}
		
			/**
			 * Returns the type of this node.
			 *
			 * @return the type of this node
			 */
			VeloxItemType getType() const {
				return m_type;
			}
		
			/**
			 * Returns the type of this node, including dereference.
			 *
			 * @return the type of this node
			 */
			VeloxItemType getXType() const {
				switch (m_type) {
				case VeloxItemType::REFERENCE:
					return m_reference->getType();
				default:
					return m_type;
				}
			}
		
			/**
			 * Creates a copy of the specified item.
			 *
			 * @param item	the item to be copied
			 * @return the copy of the item
			 */
			std::shared_ptr<VeloxItem> clone() const;
		
			/**
			 * Returns whether this item has a parent item attached.
			 *
			 * @return `true` if this item has a parent item
			 */
			bool hasParent() const;
		
			/**
			 * Returns the parent item of this item.
			 *
			 * @return the parent item or nullptr if no parent item is attached
			 */
			std::shared_ptr<VeloxItem> getParent() const;
		
			std::shared_ptr<VeloxItem> getReference() const {
				if (getType() != VeloxItemType::REFERENCE) {
					throw std::logic_error("Velox item not of reference type.");
				}
				return m_reference;
			}
		
			void addAttribute(const std::string & name, std::shared_ptr<VeloxItem> attribute);
			bool hasAttribute(const std::string & name) const;
			std::shared_ptr<VeloxItem> getAttribute(const std::string & name) const;
		
			/**
			 * Adds the specified item as array element.
			 *
			 * @param elem	the array element to add
			 */
			void addArrayElem(std::shared_ptr<VeloxItem> elem);
		
			/**
			 * Returns the array element with the specified index.
			 *
			 * @return the requested array element
			 * @throws VeloxInterpreterException in case this item is not of type array, 
			 *	of if the specified index is out of range
			 */
			std::shared_ptr<VeloxItem> getArrayElem(unsigned int index);
		
			/**
			 * Returns the array elements of this item.
			 *
			 * @return the array elements.
			 * @throws std::logic_error in the item type is invalid for this operation
			 */
			const std::vector<std::shared_ptr<VeloxItem>> & getArrayElems() const;
		
			/**
			 * Returns the number of array elements.
			 *
			 * @return the number of array elements.
			 */
			size_t numOfArrayElems() const;
		
			/**
			 * Adds attachable custom data to this item.
			 *
			 * @param attachable the custom data to attach
			 */
			void setAttachable(std::shared_ptr<IAttachable> attachable);
		
			/**
			 * Retrieves the attached custom data of this item.
			 *
			 * @return the attached custom data
			 * @throws std::logic_error in case no attachable can be retrieved
			 */
			std::shared_ptr<IAttachable> getAttachable() const;
		
			/**
			 * Retrieves the attached custom data of this item.
			 *
			 * @return the attached custom data or nullptr
			 */
			std::shared_ptr<IAttachable> findAttachable() const;
		
			/**
			 * Tests whether this item can return an attachable.
			 *
			 * @return `true` if this item can return an attachable
			 */
			bool hasAttachable() const;
		
			template<typename T>
			std::shared_ptr<T> getAttachable() const {
				std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(getAttachable());
				if (!result) {
					throw VeloxInterpreterException("Object not of type '" + std::string(typeid(T).name()) + "'");
				}
				return result;
			}
		
			template<typename T>
			std::shared_ptr<T> findAttachable() const {
				return std::dynamic_pointer_cast<T>(findAttachable());
			}
		
			template<typename T>
			bool hasAttachable() const {
				return std::dynamic_pointer_cast<T>(getAttachable()) != nullptr;
			}
		
			/**
			 * Returns a string interpretation of this item.
			 *
			 * @param the string interpretation of this item.
			 */
			std::string getStringValue(VeloxInterpreter & interpreter) const;
		
			/**
			 * Postfix '++' operator.
			 *
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator++(int);
		
			/**
			 * Postfix '--' operator.
			 *
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator--(int);
		
			/**
			 * Assignment operator.
			 *
			 * @param right	the right-hand side
			 * @return reference to this item
			 */
			VeloxItem & operator=(VeloxItem & right);
		
			/**
			 * Binary '==' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator==(const VeloxItem& right);
		
			/**
			 * Binary '==' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator!=(const VeloxItem& right);
		
			void setClosure(std::unique_ptr<VeloxScope> closure);
			VeloxScope & getClosure() const;
			bool hasClosure() const;
		
			bool isInnerFunction() const;
		
		/**
		 * Returns and casts int data of this item.
		 *
		 * @return the int representation of this item
		 */
		int getIntValue() const;
		
		/**
		 * Returns and casts double data of this item.
		 *
		 * @return the double representation of this item
		 */
		double getRealValue() const;
		
		/**
		 * Returns and casts bool data of this item.
		 *
		 * @return the bool representation of this item
		 */
		bool getBoolValue() const;
		
		/**
		 * Returns and casts std::string data of this item.
		 *
		 * @return the std::string representation of this item
		 */
		std::string getStringValue() const;
		
		/**
		 * Returns and casts VeloxHook & data of this item.
		 *
		 * @return the VeloxHook & representation of this item
		 */
		VeloxHook & getHook() const;
		
		/**
		 * Returns and casts const VeloxNode & data of this item.
		 *
		 * @return the const VeloxNode & representation of this item
		 */
		const VeloxNode & getAstNode() const;
		
			/**
			 * Unary '+'operator.
			 *
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator+() const;
		
			/**
			 * Unary '-'operator.
			 *
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator-() const;
		
			/**
			 * Unary '!'operator.
			 *
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator!() const;
		
			/**
			 * Unary '++'operator.
			 *
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator++() ;
		
			/**
			 * Unary '--'operator.
			 *
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator--() ;
		
			/**
			 * Binary '+' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator+(VeloxItem & right) const;
		
			/**
			 * Binary '-' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator-(VeloxItem & right) const;
		
			/**
			 * Binary '*' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator*(VeloxItem & right) const;
		
			/**
			 * Binary '/' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator/(VeloxItem & right) const;
		
			/**
			 * Binary '%' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator%(VeloxItem & right) const;
		
			/**
			 * Binary '<' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator<(VeloxItem & right) const;
		
			/**
			 * Binary '<=' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator<=(VeloxItem & right) const;
		
			/**
			 * Binary '>' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator>(VeloxItem & right) const;
		
			/**
			 * Binary '>=' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator>=(VeloxItem & right) const;
		
			/**
			 * Binary '&&' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator&&(VeloxItem & right) const;
		
			/**
			 * Binary '||' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator||(VeloxItem & right) const;
		
			/**
			 * Binary '+=' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator+=(VeloxItem & right) ;
		
			/**
			 * Binary '-=' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator-=(VeloxItem & right) ;
		
			/**
			 * Binary '/=' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator/=(VeloxItem & right) ;
		
			/**
			 * Binary '*=' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator*=(VeloxItem & right) ;
		
			/**
			 * Binary '%=' operator for two items.
			 *
			 * @param right	the right hand-side item
			 * @return a new item representing the result of the operation
			 */
			std::shared_ptr<VeloxItem> operator%=(VeloxItem & right) ;
		
		private:
			/** The human readable names of the item types. */
			static const std::vector<std::string> TYPE_NAMES;
		
			/**
			 * Private constructor.
			 *
			 * @param type	the type of this item
			 */
			VeloxItem(VeloxItemType type = VeloxItemType::UNDEFINED);
			VeloxItem(const VeloxItem&) = delete;
		
			/** The type of this node. */
			 VeloxItemType m_type;
		
			/** The integer of this item. */
			int m_intData;
		
			/** The integer of this item. */
			double m_realData;
		
			/** The string of this item. */
			std::string m_stringData;
		
			/** Used for hook items. */
			std::shared_ptr<VeloxHook> m_hook;
		
			/** Used for function items. */
			std::shared_ptr<const VeloxNode> m_node;
		
			/** Attributes of an object item. */
			std::unordered_map<std::string, std::shared_ptr<VeloxItem>> m_attributes;
		
			/** The parent item in case this item is an attribute. */
			std::weak_ptr<VeloxItem> m_parent;
		
			/** The referenced item. */
			std::shared_ptr<VeloxItem> m_reference;
		
			/** Custom attached data of arbitrary type. */
			std::shared_ptr<IAttachable> m_customData;
		
			/** The array elements in case this item is an array. */
			std::vector<std::shared_ptr<VeloxItem>> m_arrayElems;
		
			/** A scope representing the context of a closure function. */
			std::unique_ptr<VeloxScope> m_closure;
		
			/** Used to mark functions as inner functions. */
			bool m_innerFunction;
		
		
			std::string getArrayElemsAsString() const;
		
			/**
			 * Compares this array item with the specified one.
			 *
			 * @param right	the array on the right hand side to compare
			 * @return a boolean velox item containing the result of the comparison
			 */
			std::shared_ptr<VeloxItem> compareArrays(const VeloxItem& right);
		};
	
	} // end of namespace