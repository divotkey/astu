#include "Interpreter.h"
#include "Item.h"
#include "ItemStateFunction.h"

using namespace std;

namespace velox {

    Interpreter::Interpreter() : superGlobals(make_shared<Scope>()) {
        // Intentionally left empty.
    }

    void velox::Interpreter::Execute(std::shared_ptr<velox::InterpreterStatement> program) {
        program->Execute(context);
    }

    void Interpreter::AddFunction(const std::string &name, std::shared_ptr<InterpreterFunction> function) {
        auto item = std::make_shared<Item>(std::make_unique<ItemStateFunction>(function));
        superGlobals->AddItem(name, item);
    }

    void Interpreter::Clear() {
        context.Clear();
        context.PushScope(superGlobals);
    }

}
