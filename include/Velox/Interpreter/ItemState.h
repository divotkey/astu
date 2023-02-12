/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/ItemType.h"
#include "Graphics/Color.h"
#include "Math/Vector2.h"

// C++ Standard Library includes
#include <memory>
#include <string>

namespace velox {

    // Forward declaration
    class Item;
    class ItemStateInteger;
    class ItemStateReal;
    class ItemStateUndefined;
    class ScriptContext;
    class InterpreterActualParameterList;
    class ItemData;
    class Scope;

    class ItemState {
    public:

        /**
         * Virtual destructor.
         */
        virtual ~ItemState() {}

        /**
         * New operator allocates memory for new item states using the memory manager.
         *
         * @param count amount of bytes to allocate
         * @return the allocated memory
         */
        void * operator new(size_t count);

        /**
         * Delete operator frees memory using the memory manager.
         *
         * @param p    pointer to the allocated memory
         */
        void operator delete(void * p);

        /**
         * Carries out an assignment operation.
         *
         * @param owner the owner item of this state
         * @param rhs   the right-hand side of the assignment
         */
        void Assign(Item &owner, std::shared_ptr<Item> rhs);

        /**
         * Carries out a unary minus on this item state.
         *
         * @return a new item representing the result of the operation
         * @throws InterpreterException in case the operation is invalid for this item
         */
        virtual std::shared_ptr<Item> ExecuteUnaryMinus() const;

        /**
         * Called to evaluate this state es function.
         * The default implementation will throws an InterpreterError.
         *
         * @param sc            the script context containing the scope etc.
         * @param parameters    the actual parameters of the function call
         * @return the return value of the function call
         */
        virtual std::shared_ptr<Item>
        CallAsFunction(ScriptContext &sc, InterpreterActualParameterList &parameters, std::shared_ptr<Scope> memberScope, unsigned int lineNumber);

        /**
         * Creates a copy of this state.
         *
         * @param owner the owner of this state
         * @return the newly created copy of this state
         */
        virtual std::unique_ptr<ItemState> Copy() const = 0;

        /**
         * Tries to convert this state to a real value.
         *
         * @param lineNumber    information about the location within the source code
         * @return the real value
         * @throws InterpreterException in case this state cannot interpreted as real value
         */
        virtual double GetRealValue(unsigned int lineNumber) const;

        /**
         * Tries to convert this state to a integer value.
         *
         * @param lineNumber    information about the location within the source code
         * @return the integer value
         * @throws InterpreterException in case this state cannot interpreted as real value
         */
        virtual int GetIntegerValue(unsigned int lineNumber) const;

        /**
         * Tries to convert this state to a boolean value.
         *
         * @param lineNumber    information about the location within the source code
         * @return the boolean value
         * @throws InterpreterException in case this state cannot interpreted as boolean value
         */
        virtual bool GetBooleanValue(unsigned int lineNumber = 0) const;

        /**
         * Tries to convert this state to a string value.
         *
         * @return the string value
         * @throws InterpreterException in case this state cannot interpreted as string value
         */
        virtual std::string GetStringValue(ScriptContext &sc) const;

        /**
         * Tries to convert this state to a color value.
         *
         * @return the color value
         * @throws InterpreterException in case this state cannot interpreted as string value
         */
        virtual const astu::Color4d & GetColorValue() const;

        /**
         * Tries to get a vector2 value from this item.
         *
         * @param sc    the script context under witch to execute this operation
         * @return the extracted string value
         * @throws InterpreterException in case this item cannot interpreted as vector value
         */
        virtual const astu::Vector2d & GetVector2Value() const;

        /**
         * Looks for a sub-item with the specified name.
         *
         * @param name  the name of the sub-item
         * @return the requested item or `nullptr`
         */
        virtual std::shared_ptr<Item> FindItem(const std::string &name) const;

        /**
         * Adds an sub-item.
         *
         * @param name      the name of the item
         * @param item      the sub-item
         * @return `true` if the item has actually been added and hence the parent should be set accordingly
         */
        virtual bool AddItem(const std::string &name, std::shared_ptr<Item> item);

        /**
         * Retrieves a list element from this item (assuming that this item is of type list).
         *
         * @param idx           the index of the list element
         * @param lineNumber    the line number of this call in case of an error
         * @return the requested list element
         * @throws InterpreterError in case this state has no list elements or the index is invalid
         */
        virtual std::shared_ptr<Item> GetListElement(size_t idx, unsigned int lineNumber);

        /**
         * Returns the number of list elements of this item.
         *
         * @return the number of list elements.
         */
        virtual size_t NumListElements() const;

        /**
         * Append a list element to this item as last element.
         *
         * @param elem          the list element to append
         * @param lineNumber    the line number of this call in case of an error
         * @throws InterpreterError in case this item is not of type list
         */
        virtual void AppendListElement(std::shared_ptr<Item> elem);

        /**
         * Adds copies of this state's sub-items to the specified target.
         *
         * @param target    receives the copies of the sub-items.
         */
        virtual void CopyItems(Item& target) {}

        virtual void AddItemsToScope(Scope &scope) {}
        virtual std::shared_ptr<Item> GetParent(Item &context);

        /**
         * Returns the actual item.
         *
         * Items which are not in the reference state will return a `nullptr`, reference states will return
         * the pointer to the actual item it is referencing.
         *
         * @return the actual item without wrapper references or `nullptr`
         */
        virtual std::shared_ptr<Item> GetReferencedItem();

        /**
         * Attaches custom data to this item state.
         * The item state will delete the associated memory of the data on destruction.
         *
         * @param data   pointer to the data to attach to this item.
         */
        virtual void SetData(std::shared_ptr<ItemData> data);

        /**
         * Returns the attached data of this item.
         *
         * @return the attached data
         */
         virtual std::shared_ptr<ItemData> GetData();

        /**
         * Returns the type this item state represents.
         *
         * @return the item type.
         */
        virtual ItemType GetType() const = 0;

        /**
         * Returns whether this item state is referencing another item.
         *
         * @return `true` if this item state is a reference
         */
        virtual bool IsReference() const;

    protected:

        virtual bool Assign(Item &owner, const ItemState &rhs) {
            return false;
        }

        void SwitchState(Item& item, std::unique_ptr<ItemState> newState);
    };

}



