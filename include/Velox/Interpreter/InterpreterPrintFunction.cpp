#include "InterpreterPrintFunction.h"
#include "ItemStateUndefined.h"

#include <iostream>

using namespace std;

#define PARAM_NAME "a"

namespace velox {

    InterpreterPrintFunction::InterpreterPrintFunction() {
        AddFormalParameter(PARAM_NAME);
    }

    shared_ptr<Item> InterpreterPrintFunction::DoEvaluate(ScriptContext &sc) {
        auto &item = sc.GetItem(PARAM_NAME);
        cout << item.GetStringValue() << endl;
        return Item::Create(make_unique<ItemStateUndefined>());
    }

}
