#pragma once

#include "Interpreter.h"
#include "ScriptContext.h"
#include "Operators.h"
#include "InterpreterActualParameterList.h"
#include "ItemType.h"
#include "ItemState.h"

#include <memory>
#include <map>

namespace velox {

    // Forward declaration.
    class ItemState;

    /**
     * Items represents objects of any type within the Velox interpreter. Interpreter items
     * can change its type any time during execution. The type of an velox item is represented by its internal state.
     */
    class Item final {
    public:

        static std::shared_ptr<Item> Create(std::unique_ptr<ItemState> state) {
            return std::shared_ptr<Item>(new Item(move(state)));
        }

        Item( const Item& ) = delete; // non construction-copyable
        Item& operator=( const Item& ) = delete; // non copyable

        void * operator new(size_t count);
        void operator delete(void * p);

        /**
         * Calls this item as function.
         */
        std::shared_ptr<Item> CallAsFunction(ScriptContext &sc, InterpreterActualParameterList& parameters);

        /**
         * Assigns the contents of one statement to this another, possibly changing its type.
         * @param other the other state
         */
        void Assign(const Item& other);

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
         * @return the extracted string value
         * @throws InterpreterException in case this item cannot interpreted as string value
         */
        std::string GetStringValue() const;

        /**
         * Carries out an arithmetic operation.
         *
         * @param op    the arithmetic operator
         * @param item  the left-hand side of the operation
         * @return the result of the operation
         * @throws InterpreterException in case the operation is invalid between the two items
         */
        std::shared_ptr<Item> ExecuteArithmeticOperator(ArithmeticOperator op, const Item& item) const;

        /**
         * Carries out an relational operation.
         *
         * @param op    the relational operator
         * @param item  the left-hand side of the operation
         * @return the result of the operation
         * @throws InterpreterException in case the operation is invalid between the two items
         */
        std::shared_ptr<Item> ExecuteRelationalOperator(RelationalOperator op, const Item& item) const;

        void AddItem(const std::string& name, std::shared_ptr<Item> item);
        bool HasItem(const std::string& name) const;
        std::shared_ptr<Item> FindItem(const std::string& name);
        std::shared_ptr<const Item> FindItem(const std::string& name) const;
        Item &GetItem(const std::string& name);
        const Item &GetItem(const std::string& name) const;

    private:
        /** Lookup table for arithmetic results of items according to their types. */
        static const ItemType arithmeticResult[6][6];

        /** Lookup table defining the type for relational operations. */
        static const ItemType relationalType[6][6];

        /** The internal state of this item. */
        std::unique_ptr<ItemState> state;

        /** List of sub-items associated with unique names. */
        std::map<std::string, std::shared_ptr<Item>> subItems;

        int ExecuteIntegerArithmetic(int a, int b, ArithmeticOperator op) const;
        double ExecuteRealArithmetic(double a, double b, ArithmeticOperator op) const;
        bool ExecuteIntegerRelational(int a, int b, RelationalOperator op) const;
        bool ExecuteRealRelational(double a, double b, RelationalOperator op) const;

        /**
         * Constructor.
         */
        Item(std::unique_ptr<ItemState> state) : state(std::move(state)) {}


        friend class ItemStateReference;
    };

}


