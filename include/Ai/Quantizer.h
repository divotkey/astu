/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

namespace astu {

	/**
	 * Interface for quantizers.
	 * 
     * @ingroup ai_group
	 */
	class IQuantizer {
	public:

		/** Virtual destructor. */
		virtual ~IQuantizer() {}

		/**
		 * Quantizes the specified value.
		 *
		 * @param v	the value to quantize
		 * @return the quantized value
		 */
		virtual unsigned int Quantize(double v) = 0;

		/**
		 * Returns the maximum quantized value.
		 *
		 * @return the maximum value
		 */
		virtual unsigned GetMaxValue() const = 0;
	};

	/**
	 * A quantizer which linearly quantizes values between the lower and upper bound.
	 * 
     * @ingroup ai_group
	 */
	class LinearQuantzier : public IQuantizer {
	public:

		/**
		 * Constructor.
		 */
		LinearQuantzier(double lowerBound, double upperBound, unsigned int steps);

		/**
		 * Returns the number of steps used by this quantizer.
		 *
		 * @return number of steps
		 */
		unsigned int GetSteps() const {
			return maxValue + 1;
		}

		/**
		 * Returns the lower bound of this quantizer.
		 *
		 * @return the lower bound
		 */
		double GetLowerBound() const {
			return lowerBound;
		}

		/**
		 * Returns the upper bound of this quantizer.
		 *
		 * @return the upper bound
		 */
		double GetUpperBound() const {
			return upperBound;
		}

		// Inherited via IQuantizer
		virtual unsigned int Quantize(double v) override;
		virtual unsigned GetMaxValue() const override;

	private:
		/** The maximum value to be returned (steps - 1). */
		unsigned int maxValue;

		/** Lower boundary used to clamp values. */
		double lowerBound;

		/** Lower boundary used to clamp values. */
		double upperBound;

		/** The range of value values. */
		double range;

		/** The resolution of this quantizer. */
		double resolution;
	};

} // end of namespace