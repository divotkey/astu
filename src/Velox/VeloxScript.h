/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/VeloxTypes.h"
#include "Util/Pooled.h"

// C++ Standard Library includes
#include <string>
#include <memory>

namespace astu {

	// Forward declaration.
	class VeloxNode;
	class VeloxItem;
	class VeloxScope;
	class VeloxInterpreter;

	/**
	 * This class describes parameters for external functions calls.
	 */
	class FunctionParameters {
	public:

		/**
		 * Constructor.
		 */
		FunctionParameters();

		/**
		 * Adds the specified item as parameter.
		 *
		 * @param item the item to add
		 */
		void addParam(std::shared_ptr<VeloxItem> item);

		/**
		 * Adds an integer as parameter.
		 *
		 * @param value the integer to add
		 */
		void addParam(int value);

		/**
		 * Adds a real value as parameter.
		 *
		 * @param value the integer to add
		 */
		void addParam(Real value);

		/**
		 * Adds a string as parameter.
		 *
		 * @param s the string to add
		 */
		void addParam(const std::string & s);

		/**
		 * Returns the abstract syntax tree node representing the parameters.
		 *
		 * @param the parameter node
		 */
		const VeloxNode & getParamNode() const;

		/**
		 * Returns the parameter items.
		 *
		 * @param the parameter items
		 */
		const std::vector<std::shared_ptr<VeloxItem>> & getItems() const;

		/**
		 * Clears this function parameters.
		 */
		void clear();

	private:
		/** Used to generate temporary (almost) unique names. */
		static int m_nameCounter;

		/** The node representing the function parameters. */
		std::shared_ptr<VeloxNode> m_parameters;

		/** Stores the item parameters. */
		std::vector<std::shared_ptr<VeloxItem>> m_items;

		/**
		 * Create a new parameter name.
		 *
		 * @return	the new name
		 */
		std::string genName();
	};


	class VeloxScript final : public Pooled<VeloxScript>, public std::enable_shared_from_this<VeloxScript> {
	public:

		static std::shared_ptr<VeloxScript> create(std::shared_ptr<VeloxNode> node, const std::string & sourcePath = "") {
			return std::shared_ptr<VeloxScript>(new VeloxScript(node, sourcePath));
		}

		/**
		 * Destructor.
		 */
		~VeloxScript();

		/**
		 * Evaluates the specified function item.
		 *
		 * @param function	the function to be evaluated
		 * @throws VeloxInterpreterException in of an runtime error during execution of this script
		 * @throws std::logic_error in case in case the specified item is not a function or the script
		 *		has not been initialized
		 */
		std::shared_ptr<VeloxItem> evaluate(VeloxItem & function);

		/**
		 * Evaluates the specified function item.
		 *
		 * @param function		the function to be evaluated
		 * @param parameters	the parameters for this function
		 * @throws VeloxInterpreterException in of an runtime error during execution of this script
		 * @throws std::logic_error in case in case the specified item is not a function or the script
		 *		has not been initialized
		 */
		std::shared_ptr<VeloxItem> evaluate(VeloxItem & function, const FunctionParameters & parameters);

		/**
		 * Executes this script
		 *
		 * @throws VeloxInterpreterException in of an runtime error during execution of this script
		 * @throws std::logic_error in case this script has not been initialized
		 */
		void execute();

		/**
		 * Returns the root node of the abstract syntax tree of this script.
		 *
		 *
		 * @return the AST root node
		 */
		const VeloxNode & getRootNode() {
			return *m_node;
		}

		/**
		 * Returns the path of the source code of this script.
		 *
		 * @return the source code path
		 */
		const std::string & getSourcePath() const;

	private:
		/** The root node of the abstract syntax tree of the script. */
		std::shared_ptr<VeloxNode> m_node;

		/** The interpreter used to execute this script. */
		std::unique_ptr<VeloxInterpreter> m_interpreter;

		/** The path of the source code of the script. */
		std::string m_sourcePath;
		
		/**
		 * Constructor.
		 *
		 * @param node	the root node of the abstract syntax tree of the script
		 */
		VeloxScript(std::shared_ptr<VeloxNode> node, const std::string & sourcePath);

		// Disable copy and assignment
		VeloxScript(const VeloxScript&) = delete;
		VeloxScript & operator=(const VeloxScript&) = delete;

		/**
		 * Tests whether this script is been properly initialized.
		 *
		 * @throws std::logic_error in case this script has not been initialized
		 */
		void ensureInitialized();
	};

} // end of namespace