/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Velox/StandardVeloxHooks.h"

#define PI 3.14159265358979323846

namespace astu {

	const double ToRadiansVeloxHook::TO_RADIANS = PI / 180.0;
	const double ToDegreesVeloxHook::TO_DEGREES = 180.0 / PI;

} // end of namespace