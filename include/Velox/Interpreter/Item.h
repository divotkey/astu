#pragma once

#include "Interpreter.h"
#include "ScriptContext.h"
#include "Operators.h"
#include "InterpreterActualParameterList.h"
#include "ItemType.h"

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

        /**
         * Constructor.
         * @param state the initial state of this item.
         */
        Item(std::unique_ptr<ItemState> state);

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
         * Tries to convert this item to a string value.
         *
         * @return the extracted string value
         * @throws InterpreterException in case this item cannot interpreted as string value
         */
        std::string GetStringValue() const;

        /**
         * Carries out an arithmetic operation.
         *
         * @param op    the arithmetic operation
         * @param item  the left-hand side of the operation
         * @return the result of the operation
         * @throws InterpreterException in case the operation is invalid between the two items
         */
        std::shared_ptr<Item> ExecuteArithmeticOperator(ArithmeticOperator op, const Item& item) const;

        void AddItem(const std::string& name, std::shared_ptr<Item> item);
        bool HasItem(const std::string& name) const;
        std::shared_ptr<Item> FindItem(const std::string& name);
        std::shared_ptr<const Item> FindItem(const std::string& name) const;
        Item &GetItem(const std::string& name);
        const Item &GetItem(const std::string& name) const;

    private:
        /** Lookup table for arithmetic results of items according to their types. */
        static const ItemType arithmeticResult[6][6];

        /** The internal state of this item. */
        std::unique_ptr<ItemState> state;

        /** List of sub-items associated with unique names. */
        std::map<std::string, std::shared_ptr<Item>> subItems;

        friend class ItemStateReference;
    };

}


