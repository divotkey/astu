// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Interpreter.h"
#include "ScriptContext.h"
#include "Operators.h"
#include "InterpreterActualParameterList.h"
#include "ItemType.h"
#include "ItemState.h"
#include "ItemStateUndefined.h"
#include "ItemStateInteger.h"
#include "ItemStateReal.h"
#include "ItemStateString.h"
#include "ItemStateBool.h"
#include "ItemStateColor.h"
#include "ItemStateFunction.h"

// C++ Standard Library includes
#include <memory>
#include <map>

namespace velox {

    class ItemData {
    public:
        virtual ~ItemData() {}
    };

    template<typename T>
    class DataWrapper : public ItemData {
    public:
        DataWrapper(T data) : data(data) {}
        T data;
    };

    /**
     * Items represents objects of any type within the Velox interpreter. Interpreter items
     * can change its type any time during execution. The type of an velox item is represented by its internal state.
     */
    class Item final : public std::enable_shared_from_this<Item> {
    public:

        /**
         * Creates a new instance of an item.
         *
         * @param state the initial state of the item to create
         * @return the newly created idem
         */
        static std::shared_ptr<Item> Create(std::unique_ptr<ItemState> state) {
            return std::shared_ptr<Item>(new Item(move(state)));
        }

        /**
         * Creates a new instance of an item based on another item.
         *
         * This creation method is used to pass items as parameters to functions.
         * Some item types are copied others are wrapped as reference items.
         *
         * @param item  the source to be passed as parameter
         * @return the parameter item
         */
        static std::shared_ptr<Item> Create(std::shared_ptr<Item> item);

        /**
         * Creates a new instance of an item of type "undefined".
         *
         * @return the newly created idem
         */
        static std::shared_ptr<Item> CreateUndefined() {
            return std::shared_ptr<Item>(new Item(std::make_unique<ItemStateUndefined>()));
        }

        /**
         * Creates a new instance of an item of type "integer".
         *
         * @param value the initial integer value
         * @return the newly created idem
         */
        static std::shared_ptr<Item> CreateInteger(int value) {
            return std::shared_ptr<Item>(new Item(std::make_unique<ItemStateInteger>(value)));
        }

        /**
         * Creates a new instance of an item of type "real".
         *
         * @param value the floating-point value
         * @return the newly created idem
         */
        static std::shared_ptr<Item> CreateReal(double value) {
            return std::shared_ptr<Item>(new Item(std::make_unique<ItemStateReal>(value)));
        }

        /**
         * Creates a new instance of an item of type "string".
         *
         * @param value the string value
         * @return the newly created idem
         */
        static std::shared_ptr<Item> CreateString(const std::string& value) {
            return std::shared_ptr<Item>(new Item(std::make_unique<ItemStateString>(value)));
        }

        /**
         * Creates a new instance of an item of type "boolean".
         *
         * @param value the initial boolean value
         * @return the newly created idem
         */
        static std::shared_ptr<Item> CreateBoolean(bool value) {
            return std::shared_ptr<Item>(new Item(std::make_unique<ItemStateBool>(value)));
        }

        /**
         * Creates a new instance of an item of type "function".
         *
         * @param value the interpreter function to be executed
         * @return the newly created idem
         */
        static std::shared_ptr<Item> CreateFunction(std::shared_ptr<InterpreterFunction> value) {
            return std::shared_ptr<Item>(new Item(std::make_unique<ItemStateFunction>(value)));
        }

        /**
         * Creates a new instance of an item of type "color".
         *
         * @param value the initial color value
         * @return the newly created idem
         */
        static std::shared_ptr<Item> CreateColor(const astu::Color4d &value) {
            return std::shared_ptr<Item>(new Item(std::make_unique<ItemStateColor>(value)));
        }

        /**
         * Creates a copy of this item.
         *
         * @return  the newly created copy
         */
        std::shared_ptr<Item> Copy() const;

        Item(const Item &other) = delete; // non copyable
        Item& operator=( const Item& ) = delete; // non copyable

        /**
         * New operator allocates memory for new items using the memory manager.
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
          * Calls this item as function.
          *
          * @param sc   the script context under which this method is executed
          * @param parameters   the actual parameters for the function call
          * @param lineNumber   the line number of this function call in case of an error
          * @return the result of the function call
          */
        std::shared_ptr<Item>
        CallAsFunction(ScriptContext &sc, InterpreterActualParameterList &parameters, unsigned int lineNumber);

        /**
         * Assigns the contents of one statement to this item, possible changing its type.
         *
         * @param rhs   the right hand side of this assignment
         */
        void Assign(std::shared_ptr<Item> rhs);

        /**
         * Tries to convert this item to a real value.
         *
         * @return the real value
         * @throws InterpreterException in case this item cannot interpreted as real value
         */
        double GetRealValue() const;

        /**
         * Tries to convert this item to a integer value.
         *
         * @return the integer value
         * @throws InterpreterException in case this item cannot interpreted as integer value
         */
        int GetIntegerValue() const;

        /**
         * Tries to convert this item to a boolean value.
         *
         * @return the boolean value
         * @throws InterpreterException in case this item cannot interpreted as boolean value
         */
        bool GetBooleanValue() const;

        /**
         * Tries to convert this item to a string value.
         *
         * @param sc    the script context under witch to execute this operation
         * @return the extracted string value
         * @throws InterpreterException in case this item cannot interpreted as string value
         */
        std::string GetStringValue(ScriptContext &sc) const;

        /**
         * Carries out an arithmetic operation.
         *
         * @param sc    the the context under this this operation is executed
         * @param op    the arithmetic operator
         * @param item  the left-hand side of the operation
         * @return the result of the arithmetic operation
         * @throws InterpreterException in case the operation is invalid between the two items
         */
        std::shared_ptr<Item> ExecuteArithmeticOperator(ScriptContext &sc, ArithmeticOperator op, std::shared_ptr<Item> item) const;

        /**
         * Carries out an relational operation.
         *
         * @param sc    the the context under this this operation is executed
         * @param op    the relational operator
         * @param item  the left-hand side of the operation
         * @return the result of relational the operation
         * @throws InterpreterException in case the operation is invalid between the two items
         */
        std::shared_ptr<Item> ExecuteRelationalOperator(ScriptContext &sc, RelationalOperator op, const Item& item) const;

        /**
         * Adds an sub-item to this item.
         *
         * @param name  the name of the item
         * @param item  the sub-item
         */
        void AddItem(const std::string& name, std::shared_ptr<Item> item);

        /**
         * Looks for a sub-item with the specified name.
         *
         * @param name  the name of the sub-item
         * @return the requested item or `nullptr`
         */
        std::shared_ptr<Item> FindItem(const std::string& name);

        /**
         * Returns whether a sub-item with the specified name exists.
         *
         * @param name  the name of the sub-item
         * @return `true` if ths sub-item exists
         */
        bool HasItem(const std::string& name) const;

        /**
         * Returns the actual item.
         *
         * Items which are not in the reference state will return a `nullptr`, reference states will return
         * the pointer to the actual item it is referencing.
         *
         * @return the actual item without wrapper references or `nullptr`
         */
        std::shared_ptr<Item> GetReferencedItem();

        /**
         * Attaches custom data to this item.
         * The item will delete the associated memory of the data on destruction.
         *
         * @param data   pointer to the data to attach to this item.
         */
        void SetData(std::shared_ptr<ItemData> data);

        /**
         * Returns the attached data of this item.
         *
         * @return the attached data
         */
        std::shared_ptr<ItemData> GetData();

        Item &GetItem(const std::string& name);
        const Item &GetItem(const std::string& name) const;
        void AddItemsToScope(ScriptContext &sc) const;
        std::shared_ptr<Item> GetParent();

    private:
        /** Lookup table for arithmetic results of items according to their types. */
        static const ItemType arithmeticResult[6][6];

        /** Lookup table defining the type for relational operations. */
        static const ItemType relationalType[6][6];

        /** Lookup table defining the type for relational operations. */
        static const std::string arithmeticOperatorName[];

        /** The internal state of this item. */
        std::unique_ptr<ItemState> state;

        /** The parent of this item. */
        std::weak_ptr<Item> parent;

        /**
         * Private Constructor.
         *
         * @param state the initial state of this item
         */
        Item(std::unique_ptr<ItemState> state) : state(std::move(state)) {}

        int ExecuteIntegerArithmetic(int a, int b, ArithmeticOperator op) const;
        double ExecuteRealArithmetic(double a, double b, ArithmeticOperator op) const;
        bool ExecuteIntegerRelational(int a, int b, RelationalOperator op) const;
        bool ExecuteRealRelational(double a, double b, RelationalOperator op) const;
        bool ExecuteStringRelational(const std::string &a, const std::string &b, RelationalOperator op) const;

        friend class ItemState;
        friend class ItemStateReference;
    };

}


