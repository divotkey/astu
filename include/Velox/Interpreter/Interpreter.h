#pragma once

// Local includes
#include "InterpreterStatement.h"
#include "ScriptContext.h"
#include "Scope.h"

// C++ Standard Library includes
#include <stdexcept>
#include <memory>

namespace velox {

    class InterpreterFunction;

    class ObjectType;

    class Interpreter {

    public:

        /**
         * Constructor.
         */
        Interpreter();

        /**
         * Adds a global function that is available to all executed scripts.
         *
         * @param name      the name of the function
         * @param function  the function
         */
        void AddFunction(const std::string &name, std::shared_ptr<InterpreterFunction> function);

        /**
         * Adds an global object type (class) that is available to all executed script.
         *
         * @param name      the name of the object type
         * @param objType   the object type
         */
        void AddObjectType(const std::string &name, std::shared_ptr<ObjectType> objType);

        /**
         * Adds an global floating-point constant that is available to all executed script.
         *
         * @param name  the name of the constant
         * @param value the value of the constant
         */
        void AddRealConstant(const std::string & name, double value);

        /**
         * Adds an global integer constant that is available to all executed script.
         *
         * @param name  the name of the constant
         * @param value the value of the constant
         */
        void AddIntConstant(const std::string & name, int value);

        /**
         * Queries whether an object type has already been added.
         *
         * @param name  the name of the object type
         * @return `true` if the object type has already been added
         */
        bool HasObjectType(const std::string &name) const;



        void Execute(std::shared_ptr<InterpreterStatement> program);

        void ClearVariables();

        void ClearAll();

    private:
        std::shared_ptr<Scope> superGlobals;
        ScriptContext context;

        void AddStandardFunctions();
    };

}
