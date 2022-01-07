/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */


#pragma once

namespace astu {

	/**
	 * Interface used for things that can be attached to something without out
	 * the need to know what this something is.
	 */
	class IAttachable {
	public:

		/**
		 * Virtual destructor.
		 */
		virtual ~IAttachable() {}
	};

} // end of namespace