#pragma once

#include "Item.h"
#include "ItemType.h"

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
         * Called to evaluate this state es function.
         * The default implementation will throws an InterpreterException.
         *
         * @param sc            the script context containing the scope etc.
         * @param parameters    the actual parameters of the function call
         * @return the return value of the function call
         */
        virtual std::shared_ptr<Item> CallAsFunction(ScriptContext &sc, InterpreterActualParameterList& parameters);

        /**
         * Creates a copy of this state.
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
         * Tries to convert this state to a string value.
         *
         * @return the string value
         * @throws InterpreterException in case this state cannot interpreted as string value
         */
        virtual std::string GetStringValue() const;

        /**
         * Returns the type this item state represents.
         *
         * @return the item type.
         */
        virtual ItemType GetType() const;

    protected:

        void SwitchState(std::unique_ptr<ItemState> newState);
    };

}



