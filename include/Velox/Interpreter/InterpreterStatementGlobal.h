#pragma once

// Local includes
#include "InterpreterStatement.h"

// C++ Standard Library includes
#include <string>

namespace velox {

    class InterpreterStatementGlobal : public InterpreterStatement {
    public:

        /**
         * Constructor.
         *
         * @param lineNumber    information about the location within the source code
         */
        InterpreterStatementGlobal(unsigned int lineNumber) : InterpreterStatement(lineNumber) {}

        void SetName(const std::string &name);

        // Inherited via InterpreterStatement
        void Execute(ScriptContext &sc) override;

    private:
        /** The name of the item to declare as global. */
        std::string name;
    };

}

