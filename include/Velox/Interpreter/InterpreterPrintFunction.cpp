#include "InterpreterPrintFunction.h"
#include "ItemStateUndefined.h"

#include <iostream>

using namespace std;

#define PARAM_NAME "a"

namespace velox {

    InterpreterPrintFunction::InterpreterPrintFunction() {
        AddFormalParameter(PARAM_NAME);
    }

    shared_ptr<Item> InterpreterPrintFunction::Execute(ScriptContext &sc) {
        auto &item = sc.GetItem(PARAM_NAME);
        cout << item.GetStringValue() << endl;
        return make_shared<Item>(make_unique<ItemStateUndefined>());
    }

}
