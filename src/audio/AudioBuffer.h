/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <memory>

namespace astu {

    class AudioBuffer {
    public:

        /**
         * Constructor.
         * 
         * @param sampleRate
         * @param samples   
         * @param numSamples
         */
        AudioBuffer(double sampleRate, std::unique_ptr<float[]> samples, size_t numSamples);

        /**
         * Constructor.
         * 
         * @param sampleRate
         * @param samples   
         * @param numSamples
         */
        AudioBuffer(double sampleRate, float* samples, size_t numSamples);

        /**
         * Constructor.
         * 
         * @param sampleRate
         * @param numSamples
         */
        AudioBuffer(double sampleRate, size_t numSamples);

        /**
         * Constructor.
         * 
         * @param sampleRate
         * @param duration
         */
        AudioBuffer(double sampleRate, double duration);        

		/**
		 * Returns the sample rate.
		 *
		 * @return number of samples per second
		 */
        double GetSampleRate() const {
            return sampleRate;
        }

        /**
         * Returns the number of samples stored in this buffer.
         * 
         * @return the number of samples
         */
        unsigned int GetNumSamples() const {
            return static_cast<unsigned int>(numSamples);
        }

		/**
		 * Returns the duration of the audio data in seconds.
		 *
		 * @return the duration in seconds
		 */
		double GetDuration() const;

		/**
		 * Returns the sample at the specified index.
		 *
		 * @param index	the index of the sample to return
		 * @return the sample value at the index
		 */
		float GetSample(size_t index) const;

		/**
		 * Returns the sample at the specified time.
		 * The specified time must be within the range [0, duration).
		 *
		 * @param t	the time in seconds
		 * @return the sample value at the specified time
		 */
		double GetSample(double t) const;

    private:
        /** The sample rate of the stored audio data. */
        double sampleRate;

        /** The number of samples stored in this buffer. */
        size_t numSamples;

        /** The sample data, owned by this buffer. */
        std::unique_ptr<float[]> mySamples;

        /** Pointer to the sample data. */
        float* samples;

        void validateSampleRate();
    };

}