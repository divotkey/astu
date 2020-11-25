/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <cassert>
#include <string>
#include <stdexcept>
#include "AudioBuffer.h"

namespace astu {

    AudioBuffer::AudioBuffer(double _sampleRate, std::unique_ptr<float[]> _samples, size_t _numSamples)
        : sampleRate(_sampleRate)
        , numSamples(_numSamples)
        , mySamples(std::move(_samples))
        , samples(mySamples.get())
    {
        validateSampleRate();
    }

    AudioBuffer::AudioBuffer(double _sampleRate, float* _samples, size_t _numSamples)
        : sampleRate(_sampleRate)
        , numSamples(_numSamples)
        , samples(_samples)
    {
        validateSampleRate();
    }

    AudioBuffer::AudioBuffer(double _sampleRate, size_t _numSamples)
        : sampleRate(_sampleRate)
        , numSamples(_numSamples)
    {
        validateSampleRate();
        mySamples = std::make_unique<float[]>(numSamples);
        samples = mySamples.get();
    }

    AudioBuffer::AudioBuffer(double _sampleRate, double duration)
        : sampleRate(_sampleRate)
        , numSamples(static_cast<size_t>(sampleRate * duration + static_cast<double>(0.5)))
    {
        validateSampleRate();
        if (duration <= 0) {
            throw std::logic_error("Invalid duration for audio buffer, got " + std::to_string(duration));
        }

        mySamples = std::make_unique<float[]>(numSamples);
        samples = mySamples.get();
    }


    void AudioBuffer::validateSampleRate()
    {
        if (sampleRate <= 0) {
            throw std::domain_error("Invalid sample rate " + std::to_string(sampleRate));
        }
    }

    double AudioBuffer::GetDuration() const
    {
        return numSamples / sampleRate;
    }

    float AudioBuffer::GetSample(size_t index) const
    {
        assert(index < numSamples);
        return samples[index];
    }

    double AudioBuffer::GetSample(double t) const
    {
        double fpos = t * sampleRate;
        size_t idx = static_cast<size_t>(fpos);
        float p = static_cast<float>(fpos - idx);

        if (idx + 1 < numSamples) {
            return samples[idx] * (1.0f - p) + samples[idx + 1] * p;
        }
        else {
            return samples[idx];
        }
    }

}