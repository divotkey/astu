#include "ItemStateFunction.h"

using namespace std;

namespace velox {

    unique_ptr<ItemState> ItemStateFunction::Copy() const {
        return make_unique<ItemStateFunction>(function);
    }

    shared_ptr<Item>
    ItemStateFunction::CallAsFunction(ScriptContext &sc, InterpreterActualParameterList& parameters) {
        return function->Execute(sc, parameters);
    }


}