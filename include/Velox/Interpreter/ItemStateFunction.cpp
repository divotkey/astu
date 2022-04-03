#include "ItemStateFunction.h"

using namespace std;

namespace velox {

    shared_ptr<Item>
    ItemStateFunction::CallAsFunction(ScriptContext &sc, InterpreterActualParameterList& parameters) {
        return function->Execute(sc, parameters);
    }
    
}