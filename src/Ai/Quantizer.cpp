/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Ai/Quantizer.h"

// C++ Standard Library includes
#include <stdexcept>
#include <algorithm>
#include <string>

using namespace std;

namespace astu {

	LinearQuantzier::LinearQuantzier(double lowerBound, double upperBound, unsigned int steps)
		: maxValue(steps - 1)
		, lowerBound(lowerBound)
		, upperBound(upperBound)
		, resolution(static_cast<double>(steps))
	{
		if (steps == 0) {
			throw invalid_argument("Steps must be greater zero");
		}

		range = upperBound - lowerBound;
		if (range <= 0) {
			throw invalid_argument("Lower boundary must be less than upper boundary, got range "
				+ to_string(lowerBound) + " to " + to_string(upperBound));
		}
	}

	unsigned int LinearQuantzier::Quantize(double v)
	{
		double vt = v - lowerBound;
		return clamp(
			static_cast<unsigned int>((vt / range) * resolution), 
			static_cast<unsigned int>(0), 
			maxValue);

		//double result = clamp((vt / range) * resolution, static_cast<double>(0), resolution);
		//return static_cast<unsigned int>(result);
	}

	unsigned LinearQuantzier::GetMaxValue() const
	{
		return maxValue;
	}

} // end of namespace