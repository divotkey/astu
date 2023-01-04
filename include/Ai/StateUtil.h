/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Ai/Quantizer.h"

// C++ Standard Library includes
#include <memory>
#include <vector>

namespace astu {

	/**
	 * A utility class used to calculate a single state index
	 * for multi-dimensional states.
	 * 
     * @ingroup ai_group
	 */
	class StateUtil {
	public:

		/**
		 * Constructor.
		 */
		StateUtil();

		/**
		 * Clears all dimensions.
		 */
		void Reset();

		/**
		 * Adds a new dimension for partials states.
		 * This method does not assign any quantizer to the new dimension.
		 *
		 * @param the size of the new dimension
		 * @return the index of the new dimension
		 */
		size_t AddDimension(size_t size);

		/**
		 * Adds a new dimension for partials states.
		 * The size of the new dimension will be derived from the specified quantizer.
		 *
		 * @param quantizer the quantizer used for the new dimension
		 * @return the index of the new dimension
		 */
		size_t AddDimension(std::shared_ptr<IQuantizer> quantizer);

		/**
		 * Returns the number of dimensions.
		 *
		 * @return the number of dimensions
		 */
		size_t NumDimensions() const;

		/**
		 * Returns the size of the specified dimension.
		 *
		 * @param dimension	the dimension for which to return the size
		 * @return the size of the specified dimension
		 */
		size_t GetDimensionSize(unsigned int dimension) const;

		/**
		 * Returns the quantizer used for the specified dimension.
		 *
		 * @param dimension	the dimension for which to return the quantizer
		 * @return the requested quantizer
		 */
		std::shared_ptr<IQuantizer> GetQuantizer(unsigned int dimension);

		/**
		 * Sets the (discrete) partial state of the specified dimension.
		 *
		 * @param dimension	the dimension for which to set the state
		 * @param value	the actual state
		 */
		void SetDiscreteState(size_t dimension, unsigned int value);

		/**
		 * Sets the (continuous) partial state of the specified dimension.
		 * This method will use the associated quantizer to create
		 * the discrete state from the specified continuous value.
		 *
		 * @param dimension	the dimension for which to set the partial state
		 * @param value	the continuous value used to determine the partial state
		 */
		void SetContinuousState(size_t dimension, double value);

		/**
		 * Returns the current (discrete) partial state of the specified dimension
		 *
		 * @param dimension	the dimension for which to return the state
		 * @return the current state of the specified dimension
		 */
		unsigned int GetDiscreteState(size_t dimension) const;

		/**
		 * Returns the current (discrete) partial state of the specified dimension
		 *
		 * @param dimension	the dimension for which to return the state
		 * @return the current state of the specified dimension
		 */
		double GetContinuousState(size_t dimension) const;

		/**
		 * Returns the overall state according to the currently set partial state.
		 *
		 * @return the overall state
		 */
		size_t GetState() const;

		/**
		 * Returns the total number of states.
		 * The returned number - 1 is the highest possible overall state index.
		 *
		 * @return the total number of states.
		 */
		size_t TotalNumberOfStates() const;

	private:

		struct Dimension {
			size_t size;
			unsigned int discreteState;
			double continuousState;
			std::shared_ptr<IQuantizer> quantizer;

			Dimension(size_t s, std::shared_ptr<IQuantizer> q)
				: size(s), discreteState(0), continuousState(0), quantizer(q) 
			{
				// Intentionally left empty
			}

			void SetContinuousState(double value);
			void SetDiscreteState(unsigned int value);

		};

		/** The partials states (dimensions). */
		std::vector<Dimension> dimensions;
	};

} // end of namespace