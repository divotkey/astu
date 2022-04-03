#include "InterpreterSineFunction.h"
#include "ItemStateReal.h"
#include <cmath>

using namespace std;

#define PARAM_NAME "a"

namespace velox {

    InterpreterSineFunction::InterpreterSineFunction() {
        AddFormalParameter(PARAM_NAME);
    }

    shared_ptr<Item> InterpreterSineFunction::Execute(ScriptContext &sc) {
        auto &item = sc.GetItem(PARAM_NAME);

        double resultValue = std::sin(item.GetRealValue());
        return make_shared<Item>(make_unique<ItemStateReal>(resultValue));
    }

}
