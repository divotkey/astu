/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Util/Pooled.h"
#include "Velox/VeloxAst.h"

// C++ Standard Library includes
#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace astu {

	// Forward declaration
	class VeloxItem;
	class VeloxScope;
	class VeloxScript;
	class VeloxHook;
	class IVeloxImporter;
	class FunctionParameters;

	class VeloxInterpreter {
	public:

		/**
		 * Obtains a new velox interpreter instance.
		 *
		 * @param root
		 * @param sourcePath 
		 **/
		static std::unique_ptr<VeloxInterpreter> Obtain(std::shared_ptr<VeloxNode> root, const std::string& sourcePath = "");

		/** Puts a velox interpreter instance back into the interpreter pool. */
		static void Free(std::unique_ptr<VeloxInterpreter> interpreter);

		/**
		 * Constructor.
		 */
		VeloxInterpreter();

		/**
		 * Destructor.
		 */
		~VeloxInterpreter();

		/**
		 * Resets this interpreter to its initial condition.
		 */
		void reset();

		/**
		 * Executes the current script.
		 */
		void execute();

		/**
		 * Evaluates the specified function item.
		 *
		 * @param function	the function to be evaluated
		 * @throws VeloxInterpreterException in case of an runtime error of the script
		 * @throws std::logic_error in case the specified item is not a function or
		 *	the interpreter has not been initialized.
		 */
		std::shared_ptr<VeloxItem> evaluate(const VeloxItem & function);

		/**
		 * Evaluates the specified function item.
		 *
		 * @param function	the function to be evaluated
		 * @param params	the parameters for this function
		 * @throws VeloxInterpreterException in case of an runtime error of the script
		 * @throws std::logic_error in case the specified item is not a function or
		 *	the interpreter has not been initialized.
		 */
		std::shared_ptr<VeloxItem> evaluate(
			const VeloxItem & function, 
			const FunctionParameters & params
			);


		void setPrintFunc(std::function<void(const std::string &)> func);
		void setWarnFunc(std::function<void(const std::string &)> func);

		/**
		 * Sets the importer used to carry out import statements.
		 *
		 * @param importer	the importer to use
		 */
		void setImporter(std::shared_ptr<IVeloxImporter> importer);

		bool hasHook(const std::string & name);
		void addHook(std::shared_ptr<VeloxHook> hook);
		void addHook(const std::string & name, std::shared_ptr<VeloxHook> hook);
		void removeHook(const std::string & name);

	private:
		/** The pool of velox interpreter. */
		static std::vector<std::unique_ptr<VeloxInterpreter>> pool;

		enum class ExitType { RETURN, BREAK, CONTINUE, VOID, EXIT, YIELD };

		/** The current scope to be used. */
		std::unique_ptr<VeloxScope> m_scope;

		// std::weak_ptr<VeloxScript> m_script;
		std::shared_ptr<VeloxNode> m_rootNode;

		/** The file path to the script source. */
		std::string m_sourcePath;

		std::weak_ptr<IVeloxImporter> m_importer;

		/**
		 * Initializes this interpeter for execution.
		 * 
		 * @param root			the root AST node of the script to execute
		 * @param sourcePath	the filepath to the script source code
		 */
		void Initialize(std::shared_ptr<VeloxNode> root, const std::string& sourcePath);

		/** Built-in functions realized as Velox script rather than as hook. */
		std::unordered_map <std::string, std::shared_ptr<VeloxNode>> m_functions;

		void functionLookahead(const VeloxNode & node, bool innerFunction);
		std::function<void(const std::string & s)> m_printFunc;
		std::function<void(const std::string & s)> m_warnFunc;
		std::vector <std::shared_ptr<VeloxItem>> m_returnStack;
		std::unordered_map<std::string, std::shared_ptr<VeloxHook>> m_hooks;

		void openScope();
		void openBlockScope();
		void closeScope();
		void closeAllScopesButFirst();

		std::shared_ptr<VeloxItem> evaluate(const VeloxNode & node);
		std::shared_ptr<VeloxItem> evaluateInfix(const VeloxNode & node);
		std::shared_ptr<VeloxItem> evaluatePrefix(const VeloxNode & node);
		std::shared_ptr<VeloxItem> evaluatePostfix(const VeloxNode & node);
		std::shared_ptr<VeloxItem> evaluateUnary(const VeloxNode & node);
		std::shared_ptr<VeloxItem> evaluateAssignment(const VeloxNode & node);
		std::shared_ptr<VeloxItem> evaluateRelation(const VeloxNode & node);
		std::shared_ptr<VeloxItem> evaluateLeftHandSide(const VeloxNode & node);
		std::shared_ptr<VeloxItem> evaluateArrayInitializer(const VeloxNode & node);
		std::shared_ptr<VeloxItem> evaluateFieldAccess(const VeloxNode & node, bool leftHandSide);
		std::shared_ptr<VeloxItem> evaluateArrayAccess(const VeloxNode & node);
		std::shared_ptr<VeloxItem> evaluateSimpleName(const VeloxNode & node, bool leftHandSide);
		std::shared_ptr<VeloxItem> evaluateGlobalSimpleName(const VeloxNode & node);
		std::shared_ptr<VeloxItem> evaluateFunctionCall(const VeloxNode & node);
		std::shared_ptr<VeloxItem> evaluateObjectCreation(const VeloxNode & node);
		std::shared_ptr<VeloxItem> evaluateScriptFunctionCall(const VeloxItem & item, const VeloxNode & params);
		std::shared_ptr<VeloxItem> evaluateHookFunctionCall(const VeloxItem & item, const VeloxNode & params);

		void interpretPrint(const VeloxNode & node);
		ExitType interpretBlock(const VeloxNode & node);
		ExitType interpretSequence(const VeloxNode & node);
		ExitType interpretIf(const VeloxNode & node);
		void interpretReturn(const VeloxNode & node);
		ExitType interpretWhile(const VeloxNode & node);
		ExitType interpretFor(const VeloxNode & node);
		ExitType interpretStatement(const VeloxNode & node);
		ExitType interpretImport(const VeloxNode & node);
		void interpretExpressionStatement(const VeloxNode & node);
		void interpretFunctionDeclaration(const VeloxNode & node, bool innerFunction = false);
		void interpretClassDeclaration(const VeloxNode & node);
		void emitWarning(const std::string & msg);
		void addHooks(VeloxScope& scope);

		std::shared_ptr<VeloxItem> popReturn();
		void pushReturn(std::shared_ptr<VeloxItem> item);
		VeloxItem& peekReturn();
	};

} // end of namespace
