/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Ai/StateUtil.h"

// C++ Standard Library includes
#include <cassert>

namespace astu {

	StateUtil::StateUtil()
	{
		// Intentionally left empty.
	}

	void StateUtil::Reset()
	{
		dimensions.clear();
	}

	size_t StateUtil::AddDimension(size_t size)
	{
		dimensions.push_back(Dimension(size, nullptr));
		return dimensions.size() - 1;
	}

	size_t StateUtil::AddDimension(std::shared_ptr<IQuantizer> quantizer)
	{
		dimensions.push_back(Dimension(quantizer->GetMaxValue() + 1, quantizer));
		return dimensions.size() - 1;
	}

	size_t StateUtil::NumDimensions() const
	{
		return dimensions.size();
	}

	size_t StateUtil::GetDimensionSize(unsigned int dimension) const
	{
		assert(dimension < dimensions.size());
		return dimensions[dimension].size;
	}

	std::shared_ptr<IQuantizer> StateUtil::GetQuantizer(unsigned int dimension)
	{
		assert(dimension < dimensions.size());
		return dimensions[dimension].quantizer;
	}

	void StateUtil::SetDiscreteState(size_t dimension, unsigned int value)
	{
		assert(dimension < dimensions.size());
		dimensions[dimension].SetDiscreteState(value);
	}

	void StateUtil::SetContinuousState(size_t dimension, double value)
	{
		assert(dimension < dimensions.size());
        dimensions[dimension].SetContinuousState(value);
	}

	unsigned int StateUtil::GetDiscreteState(size_t dimension) const
	{
		assert(dimension < dimensions.size());
		return dimensions[dimension].discreteState;
	}

	double StateUtil::GetContinuousState(size_t dimension) const
	{
		assert(dimension < dimensions.size());
		return dimensions[dimension].continuousState;
	}

	size_t StateUtil::GetState() const
	{
		size_t s = 0, m = 1;
		for (auto & dimension : dimensions) {
			s += dimension.discreteState * m;
			m *= dimension.size;
		}
		return s;
	}

	size_t StateUtil::TotalNumberOfStates() const
	{
		size_t m = 1;
		for (auto & dimension : dimensions) {
			m *= dimension.size;
		};
		return m;
	}

	void StateUtil::Dimension::SetContinuousState(double value)
	{
		assert(quantizer);
		SetDiscreteState(quantizer->Quantize(value));
		continuousState = value;
	}

	void StateUtil::Dimension::SetDiscreteState(unsigned int value)
	{
		discreteState = value;
	}

} // end of namespace