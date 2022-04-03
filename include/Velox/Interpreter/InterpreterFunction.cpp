#include "InterpreterFunction.h"
#include "Scope.h"
#include "InterpreterException.h"
#include "ItemStateUndefined.h"

#include <cassert>

using namespace std;

namespace velox {

    shared_ptr<Item>
    InterpreterFunction::Execute(ScriptContext &sc, InterpreterActualParameterList &actualParameters) {
        // Actual parameters must be less or equal to formal parameters.
        if (formalParameters.size() < actualParameters.NumParameters()) {
            throw InterpreterException("Function call with to many parameters");
        }

        // Create a new scope for the function parameters.
        auto parameterScope = make_shared<Scope>();

        // Fill scope with parameters, either actual evaluated params or undefined.
        for (size_t i = 0; i < formalParameters.size(); ++i) {
            std::shared_ptr<Item> parameter;

            if (i < actualParameters.NumParameters()) {
                parameter = actualParameters.GetParameter(i).Evaluate(sc);
            } else {
                parameter = make_shared<Item>(make_unique<ItemStateUndefined>());
            }

            parameterScope->AddItem(formalParameters[i], parameter);
        }

        // Script context must contain parameters before actual function is evaluated.
        sc.PushScope(parameterScope);

        // Execute/evaluate the actual function.
        auto result = Execute(sc);

        // Clean up the scope stack.
        sc.PopScope();

        return nullptr;
    }

    void InterpreterFunction::AddFormalParameter(const string &simpleName) {
        assert(!HasFormalParameter(simpleName));
        formalParameters.push_back(simpleName);
    }

    bool InterpreterFunction::HasFormalParameter(const string &simpleName) const {
        return find(formalParameters.begin(), formalParameters.end(), simpleName) != formalParameters.end();
    }

}
