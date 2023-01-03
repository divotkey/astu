/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library include
#include <stdexcept>
#include <cmath>

namespace astu {

    /**
     * Interface for interpolators for one-dimensional values.
     * 
     * @tparam T    the numerical type used for the values to interpolate
     * @ingroup math_group
     */
    template<typename T>
    class Interpolator1 {
    public:

        /**
         * Constructor.
         * 
         * @param initialValue  used to initialize target and current value
         */
        Interpolator1(T initialValue) {
            Reset(initialValue);
        }

        /**
         * Sets the new target value.
         * 
         * @param newTarget the new target value
         */
        void SetTargetValue(T newTarget) {
            targetValue = newTarget;
        }

        /**
         * Returns the latest set target value.
         * 
         * @return the target value
         */
        T GetTargetValue() const {
            return targetValue;
        }

        /**
         * Returns the current value.
         * 
         * @return  the current value based on target value and interpolation
         */
        T GetCurrentValue() const {
            return currentValue;
        }

        /**
         * Resets the current and target value to the specified value.
         * 
         * @param value the value for target and current value
         */
        virtual void Reset(T value) {
            targetValue = value;
            currentValue = value;
        }

        /**
         * Updates the current value.
         * 
         * @param dt    the elapsed time in seconds
         * @return the current value
         */
        virtual T Update(T dt) = 0;

        /**
         * Convenient method to set the target value and update the current value.
         * 
         * @param targetValue   the new target value
         * @param dt            the elapsed time since the last update in seconds
         */
        T SetTargetAndUpdate(T targetValue, T dt) {
            SetTargetValue(targetValue);
            return Update(dt);
        }

    protected:
        /** The current value. */
        T currentValue;

        /** The target value. */
        T targetValue;
    };

    /**
     * Interpolates between two one-dimensional values linearily.
     * 
     * @tparam T    the numerical type used for the values to interpolate
     * @ingroup math_group
     */
    template<typename T>
    class LinearInterpolator1 : public Interpolator1<T>  {
    public:

        /**
         * Constructor.
         * 
         * @param speed         the interpolation speed
         * @param initialValue  the initial value of this interpolator
         */
        LinearInterpolator1(T speed = static_cast<T>(1), T initialValue = static_cast<T>(0))
            : Interpolator1<T>(initialValue)
        {
            SetSpeed(speed);
        }

        /**
         * Sets the interpolation speed of this interpolator.
         * The interpolation speed determines the speed used to 
         * reach the target value.
         * 
         * @param newSpeed  the new speed
         */
        void SetSpeed(T newSpeed) {
            if (newSpeed <= 0) {
                throw std::domain_error("Interpolation speed must be gerater zero");
            }

            speed = newSpeed;
        }

        virtual T Update(T dt) override {
            // Calc difference (error) between current value and target value.
            const T e = Interpolator1<T>::targetValue - Interpolator1<T>::currentValue;

            // Determine velocity of value change.
            float v = (e == 0.0) ? 0.0f : std::copysignf(speed, e);

            // Update and current value.
            Interpolator1<T>::currentValue += v * dt;

            // Test is error has changed sign. If that happens we did overshoot
            // the target value.
            if ((Interpolator1<T>::targetValue - Interpolator1<T>::currentValue) * e < 0) {
                Interpolator1<T>::currentValue = Interpolator1<T>::targetValue;
            }

            return Interpolator1<T>::currentValue;
        }

    private:
        /** The speed used to reach the target value .*/
        T speed;
    };

    using LinearInterpolator1f = LinearInterpolator1<float>;
    using LinearInterpolator1d = LinearInterpolator1<double>;

} // end of namespace