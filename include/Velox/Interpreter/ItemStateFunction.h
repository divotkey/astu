#pragma once

#include "Item.h"
#include "ItemState.h"
#include "InterpreterFunction.h"

namespace velox {

    class ItemStateFunction : public ItemState {
    public:

        ItemStateFunction(std::shared_ptr<InterpreterFunction> function) : function(function) {};

        std::unique_ptr<ItemState> Copy() const override;

        std::shared_ptr<Item> CallAsFunction(ScriptContext &sc, InterpreterActualParameterList& parameters) override;

    private:
        std::shared_ptr<InterpreterFunction> function;
    };

}



