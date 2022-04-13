// Local includes
#include "InterpreterSineFunction.h"
#include "ItemStateReal.h"
#include "Item.h"

// C++ Standard Library includes
#include <cmath>

using namespace std;

#define PARAM_NAME "a"

namespace velox {

    InterpreterSineFunction::InterpreterSineFunction() {
        AddFormalParameter(PARAM_NAME);
    }

    shared_ptr<Item> InterpreterSineFunction::DoEvaluate(ScriptContext &sc) {
        auto &item = sc.GetItem(PARAM_NAME);

        double resultValue = std::sin(item.GetRealValue());
        return Item::Create(make_unique<ItemStateReal>(resultValue));
    }

}
