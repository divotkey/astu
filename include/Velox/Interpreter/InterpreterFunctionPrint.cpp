// Local includes
#include "InterpreterFunctionPrint.h"
#include "ItemStateUndefined.h"
#include "Item.h"

// C++ Standard Library includes
#include <iostream>

using namespace std;

#define PARAM_NAME "a"

namespace velox {

    InterpreterFunctionPrint::InterpreterFunctionPrint() {
        AddFormalParameter(PARAM_NAME);
    }

    std::shared_ptr<Item> InterpreterFunctionPrint::DoEvaluate(ScriptContext &sc, unsigned int lineNumber) {
        auto &item = sc.GetItem(PARAM_NAME);
        cout << item.GetStringValue(sc) << endl;
        return Item::Create(make_unique<ItemStateUndefined>());
    }

}
