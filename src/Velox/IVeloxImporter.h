/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <memory>
#include <string>

namespace astu {

	// Forward declaration
	class VeloxNode;

	class IVeloxImporter {
	public:

		/**
		 * Virtual destructor.
		 */
		virtual ~IVeloxImporter() {}

		/**
		 * Imports the abstract syntax tree from of the script with the specified name
		 *
		 * @param filename		the name of the script file to import
		 * @param sourcePath	the source path of the script issuing the import statement
		 */
		virtual std::shared_ptr<VeloxNode> importAst(const std::string & filename, const std::string & sourcePath = "") = 0;
	};

} // end of namespace
