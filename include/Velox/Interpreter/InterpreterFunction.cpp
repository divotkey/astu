#include "InterpreterFunction.h"
#include "Scope.h"
#include "InterpreterError.h"
#include "ItemStateUndefined.h"

// C++ Standard Library
#include <cassert>
#include <algorithm>
#include <string>

using namespace std;

namespace velox {

    std::shared_ptr<Item>
    InterpreterFunction::Evaluate(ScriptContext &sc, InterpreterActualParameterList &actualParameters,
                                  unsigned int lineNumber)
    {
        // Actual parameters must be less or equal to formal parameters.
        if (formalParameters.size() < actualParameters.NumParameters()) {
            throw InterpreterError(string("Function call with to many parameters (expected ") +
                                   std::to_string(formalParameters.size()) + " but got " +
                                   to_string(actualParameters.NumParameters()) + ")", lineNumber);
        }

        // Create a new scope for the function parameters.
        auto parameterScope = make_shared<Scope>();

        // Fill scope with parameters, either actual evaluated params or undefined.
        for (size_t i = 0; i < formalParameters.size(); ++i) {
            std::shared_ptr<Item> parameter;

            if (i < actualParameters.NumParameters()) {
                parameter = Item::Create(actualParameters.GetParameter(i).Evaluate(sc));
            } else {
                parameter = Item::Create(make_unique<ItemStateUndefined>());
            }

            parameterScope->AddItem(formalParameters[i], parameter);
        }

        // Script context must contain parameters before actual function is evaluated.
        sc.PushScope(parameterScope);

        // DoEvaluate/evaluate the actual function.
        auto result = DoEvaluate(sc);

        // Clean up the scope stack.
        sc.PopScope();

        return result;
    }

    void InterpreterFunction::AddFormalParameter(const string &simpleName) {
        assert(!HasFormalParameter(simpleName));
        formalParameters.push_back(simpleName);
    }

    bool InterpreterFunction::HasFormalParameter(const string &simpleName) const {
        return find(formalParameters.begin(), formalParameters.end(), simpleName) != formalParameters.end();
    }

}
