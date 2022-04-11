// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Item.h"
#include "ItemType.h"

// C++ Standard Library includes
#include <memory>

namespace velox {

    // Forward declaration
    class Item;
    class ItemStateInteger;
    class ItemStateReal;
    class ItemStateUndefined;
    class ScriptContext;
    class InterpreterActualParameterList;

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
         * Called to evaluate this state es function.
         * The default implementation will throws an InterpreterError.
         *
         * @param sc            the script context containing the scope etc.
         * @param parameters    the actual parameters of the function call
         * @return the return value of the function call
         */
        virtual std::shared_ptr<Item>
        CallAsFunction(ScriptContext &sc, InterpreterActualParameterList &parameters, unsigned int lineNumber);

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
         * @return the real value
         * @throws InterpreterException in case this state cannot interpreted as real value
         */
        virtual double GetRealValue() const;

        /**
         * Tries to convert this state to a integer value.
         *
         * @return the integer value
         * @throws InterpreterException in case this state cannot interpreted as real value
         */
        virtual int GetIntegerValue() const;

        /**
         * Tries to convert this state to a boolean value.
         *
         * @return the boolean value
         * @throws InterpreterException in case this state cannot interpreted as boolean value
         */
        virtual bool GetBooleanValue() const;

        /**
         * Tries to convert this state to a string value.
         *
         * @return the string value
         * @throws InterpreterException in case this state cannot interpreted as string value
         */
        virtual std::string GetStringValue() const;

        /**
         * Looks for a sub-item with the specified name.
         *
         * @param name  the name of the sub-item
         * @return the requested item or `nullptr`
         */
        virtual std::shared_ptr<Item> FindItem(const std::string &name);

        /**
         * Adds an sub-item.
         *
         * @param name      the name of the item
         * @param item      the sub-item
         * @return `true` if the item has actually been added and hence the parent should be set accordingly
         */
        virtual bool AddItem(const std::string &name, std::shared_ptr<Item> item);

        /**
         * Adds copies of this state's sub-items to the specified target.
         *
         * @param target    receives the copies of the sub-items.
         */
        virtual void CopyItems(Item& target) {}

        virtual void AddItemsToScope(ScriptContext &sc) {}
        virtual std::shared_ptr<Item> GetParent(Item &context);


        /**
         * Returns the type this item state represents.
         *
         * @return the item type.
         */
        virtual ItemType GetType() const;

    protected:

        virtual bool Assign(Item &owner, const ItemState &rhs) {
            return false;
        }

        void SwitchState(Item& item, std::unique_ptr<ItemState> newState);
    };

}



