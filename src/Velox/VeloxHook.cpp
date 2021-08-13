/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Velox/VeloxHook.h"
#include "Velox/VeloxInterpreter.h"
#include "Velox/VeloxScope.h"
#include "Velox/VeloxItem.h"

namespace astu {

	VeloxHook::VeloxHook(const std::string & name)
		: m_name(name)
	{
		// Intentionally left empty.
	}

	const std::string & VeloxHook::getName() const
	{
		return m_name;
	}

	void VeloxHook::addParam(const std::string & s)
	{
		m_formalParams.push_back(s);
	}

	double VeloxHook::getDouble(VeloxScope & scope, const std::string & name)
	{
		return scope.getItem(name)->getRealValue();
	}

	Real VeloxHook::getReal(VeloxScope & scope, const std::string & name)
	{
		return static_cast<Real>(scope.getItem(name)->getRealValue());
	}

} // end of namespace