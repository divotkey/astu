/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#define _USE_MATH_DEFINES
 
#include <cassert>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <limits>

#include "audio/WaveCodec.h"
#include "audio/AudioBuffer.h"
#include "math/Random.h"

#include "AstUtilsConfig.h"
#include "AstUtils0.h"

using namespace astu;
using namespace std;

/////////////////////////////////////////////////
/////// Constants
/////////////////////////////////////////////////
const double TO_RADIANS = M_PI / 180.0;
const double TO_DEGREES = 180.0 / M_PI;
const char* kErrorMessages[] = {
    "No Error", 
    "Invalid parameter",
    "Unable to open file for reading", 
    "Unable to open file for writing",
    "Unable to read file",
    "Unable to import file, invalid format?",
    "Invalid error code",
    };

/////////////////////////////////////////////////
/////// Globals
/////////////////////////////////////////////////
std::chrono::time_point<std::chrono::steady_clock> startTime;
std::chrono::time_point<std::chrono::steady_clock> stopTime;
int gLastError = ErrorCode::NO_ERROR;
std::string gErrorDetails;

/////////////////////////////////////////////////
/////// Error functions
/////////////////////////////////////////////////

int GetLastError()
{
    return gLastError;
}

const char* GetErrorMessage(int errorCode)
{
    if (errorCode < 0 || errorCode > UNKNOWN_ERROR_CODE) {
        return kErrorMessages[ErrorCode::UNKNOWN_ERROR_CODE];
    }

    return kErrorMessages[errorCode];
}

void SetLastError(int errorCode)
{
    gLastError = errorCode;
    gErrorDetails.clear();
}

void SetErrorDetails(const std::string& detail)
{
    gErrorDetails = detail;
}

const char* GetErrorDetails()
{
    return gErrorDetails.c_str();
}

/////////////////////////////////////////////////
/////// I/O Functions
/////////////////////////////////////////////////

void SayHello()
{
    cout << "Hello AST World :-)" << endl;
}

void SayText(const char* text, bool eol)
{
    if (text) {
        std::cout << text;
    }
    if (eol) {
        std::cout << std::endl;
    }
}

void SayInt(int value, bool eol)
{
    std::cout << value;
    if (eol) {
        std::cout << std::endl;
    }
}

void SayDouble(double value, bool eol)
{
    std::cout << value;
    if (eol) {
        std::cout << std::endl;
    }
}

void SayVersion() {

    cout << "AST Utilities Version " 
        << ASTU_VERSION_MAJOR << "." 
        << ASTU_VERSION_MINOR << "."
        << ASTU_VERSION_PATCH
        << " (" << ((sizeof(void*) * 8)) << " bit address space)" 
        << " [build: " << __DATE__ <<", " << __TIME__ << "]"
        << endl;
}

void SayElapsedTime(const char* text)
{
    cout << text << " ";

    auto dt = chrono::duration_cast<chrono::milliseconds>(stopTime - startTime).count();

    if (dt == 0) {
        dt = chrono::duration_cast<chrono::nanoseconds>(stopTime - startTime).count();
        double dtms = (dt / 1000000.0);
        cout << dtms << " ms" << endl;
        return;
    }

    int hr = dt / (1000 * 60 * 24);
    dt -= hr * (1000 * 60 * 24);

    if (hr > 0) {
        cout << hr << " hr ";
    }

    int min = dt / (1000 * 60);
    dt -= min * (1000 * 60);

    if (min > 0 || hr > 0) {
        cout << min << " min ";
    }

    int sec = dt / 1000;
    dt -= sec * 1000;

    if (sec > 0 || hr > 0 || min > 0) {
        cout << sec << " sec ";
    }
    if (dt > 0) {
        cout << dt << " ms";
    }

    cout << endl;
}

int AskInt(const char* text)
{
    int result;
    if (text) {
        cout << text << " ";
    }
    cin >> result;

    return result;
}

double AskDouble(const char* text)
{
    double result;
    if (text) {
        cout << text << " ";
    }
    cin >> result;

    return result;
}


/////////////////////////////////////////////////
/////// Math Functions
/////////////////////////////////////////////////

double ToRadians(double deg)
{
    return deg * TO_RADIANS;
}

double ToDegrees(double rad)
{
    return rad * TO_DEGREES;
}

double GetRandomDouble(double minValue, double maxValue)
{
    if (minValue == 0 && maxValue == 1) {
        return Random::GetInstance().NextDouble();
    } else {
        return minValue + Random::GetInstance().NextDouble() * (maxValue - minValue);
    }
}

int RoundToInt(double value)
{
    return static_cast<int>(value + 0.5);
}

/////////////////////////////////////////////////
/////// Timer Functions
/////////////////////////////////////////////////

void StartTimer()
{
    startTime = stopTime = std::chrono::steady_clock::now();
}

void StopTimer()
{
    stopTime = chrono::steady_clock::now();
}

int GetMilliseconds()
{
    auto dt = chrono::duration_cast<chrono::milliseconds>(stopTime - startTime).count();
    return static_cast<int>(dt);
}

/////////////////////////////////////////////////
/////// Audio Functions
/////////////////////////////////////////////////

bool write4ByteInt(std::ostream& os, uint32_t value)
{
    os.write(reinterpret_cast<const char*>(&value), sizeof(uint32_t));
    return os.good();
}

bool write2ByteInt(std::ostream& os, uint16_t value)
{
    os.write(reinterpret_cast<const char*>(&value), sizeof(uint16_t));
    return os.good();
}

int WriteAudio(const char * filename, float *data, int size, int sampleRate, int channels)
{
    // Create file output stream.
    std::ofstream ofs(filename, std::ios::out | std::ios::binary);
    if (!ofs) {
        return ErrorCode::UNABLE_TO_OPEN_FILE_FOR_WRITING;
    }

    FormatChunk formatChunk;
    formatChunk.SetAudioFormat(FormatChunk::AudioFormat::IEEE_FLOAT);
    formatChunk.SetBitsPerSamples(32);
    formatChunk.SetNumberOfChannels(static_cast<unsigned int>(channels));
    formatChunk.SetSampleRate(static_cast<unsigned int>(sampleRate));
    formatChunk.Update();

    DataChunk dataChunk(size * sizeof(float));

    WaveChunk waveChunk(formatChunk.GetSize() + dataChunk.GetSize());
    waveChunk.Write(ofs);
    formatChunk.Write(ofs);
    dataChunk.Write(ofs);

    ofs.write(reinterpret_cast<const char*>(data), dataChunk.GetDataSize());

    // Close stream.
    ofs.close();

    return ErrorCode::NO_ERROR;
}

bool FindChunk(std::istream& is, const char *fourcc)
{

    SkipChunk skip;

    skip.ReadFourCC(is);
    while (!skip.IsFourCC(fourcc) && is.good()) {
        skip.ReadWithoutFourCC(is);
        skip.ReadFourCC(is);
    }
    return is.good();
}

float *ReadAudio(const char* filename, int* size, int *sampleRate, int *channels)
{
    if (!filename || !size || !sampleRate || !channels)
    {
        SetLastError(ErrorCode::INVALID_PARAMETER);
        return nullptr;
    }

    // Open input stream.
    std::ifstream ifs(filename, std::ios::in | std::ios::binary);

    if (!ifs.good()) {
        SetLastError(ErrorCode::UNABLE_TO_OPEN_FILE_FOR_READING);
        return nullptr;
    }

    // Create utility objects used to read RIFF chunks.
    WaveChunk waveChunk;
    FormatChunk formatChunk;
    DataChunk dataChunk;

    try {
        // Read main chunk.
        waveChunk.Read(ifs);


        // Read audio format chunk.
        if (!FindChunk(ifs, formatChunk.GetFourCC())) {
            SetLastError(ErrorCode::UNABLE_TO_IMPORT_FILE);
            SetErrorDetails("Format chunk not found");
            ifs.close();
            return nullptr;
        }
        formatChunk.ReadWithoutFourCC(ifs);

        // Read data chunk (without actual audio data).
        if (!FindChunk(ifs, dataChunk.GetFourCC())) {
            SetLastError(ErrorCode::UNABLE_TO_IMPORT_FILE);
            SetErrorDetails("Data chunk not found");
            ifs.close();
            return nullptr;
        }
        dataChunk.ReadWithoutFourCC(ifs);

    } catch (std::runtime_error & e) {
        SetLastError(ErrorCode::UNABLE_TO_IMPORT_FILE);
        SetErrorDetails(e.what());
        ifs.close();
        return nullptr;
    }

    // Set output parameter to audio format.
    *sampleRate = formatChunk.GetSampleRate();
    *channels = formatChunk.GetNumberOfChannels();

    // Read audio data.
    std::unique_ptr<float[]> data;
    if (formatChunk.GetAudioFormat() == FormatChunk::AudioFormat::IEEE_FLOAT) {
        // No conversion required, read plain floating-point values.

        assert(dataChunk.GetDataSize() % sizeof(float) == 0);
        *size = static_cast<unsigned int>(dataChunk.GetDataSize() / sizeof(float));
        data = std::make_unique<float[]>(*size);
        ifs.read(reinterpret_cast<char*>(data.get()), dataChunk.GetDataSize());
    } else if (formatChunk.GetAudioFormat() == FormatChunk::AudioFormat::PCM) {
        // We need to convert 8-bit or 16-bit PCM to IEEE float.
        
        if (formatChunk.GetBitsPerSample() == 16) {
            // Convert 16-bit samples to float.
    
            assert(dataChunk.GetDataSize() % sizeof(int16_t) == 0);
            *size = static_cast<unsigned int>(dataChunk.GetDataSize() / sizeof(int16_t));
            std::unique_ptr<int16_t[]> temp = std::make_unique<int16_t[]>(*size);
            ifs.read(reinterpret_cast<char*>(temp.get()), dataChunk.GetDataSize());

            data = std::make_unique<float[]>(*size);
            int maxValue = -std::numeric_limits<int16_t>::min();

            const float kToFloat = -1.0f / std::numeric_limits<int16_t>::min();
            for (unsigned int i = 0; i < *size; ++i) {
                data[i] = temp[i] * kToFloat;
                assert(data[i] <= 1.0f || data[i] >= -1.0f);
            }
        } else if(formatChunk.GetBitsPerSample() == 24) {
            // Convert 24-bit samples to float.
            assert(dataChunk.GetDataSize() % 3 == 0);

            *size = static_cast<unsigned int>(dataChunk.GetDataSize() / 3);
            std::unique_ptr<uint8_t[]> temp = std::make_unique<uint8_t[]>(dataChunk.GetDataSize());
            ifs.read(reinterpret_cast<char*>(temp.get()), dataChunk.GetDataSize());

            data = std::make_unique<float[]>(*size);
            const float kToFloat = 1.0f / 8388608;

            for (unsigned int i = 0; i < *size; ++i) {
                data[i] = (temp[i * 3] << 8 | temp[i * 3 + 1] << 16 | temp[i * 3 + 2] << 24) / 2147483648.0f;
                assert(data[i] <= 1.0f || data[i] >= -1.0f);
            }

        } else if (formatChunk.GetBitsPerSample() == 8) {
            // Convert 8-bit samples to float.

            *size = static_cast<unsigned int>(dataChunk.GetDataSize());
            std::unique_ptr<uint8_t[]> temp = std::make_unique<uint8_t[]>(*size);
            ifs.read(reinterpret_cast<char*>(temp.get()), dataChunk.GetDataSize());

            data = std::make_unique<float[]>(*size);

            const float kToFloat = 2.0f / std::numeric_limits<uint8_t>::max();
            for (unsigned int i = 0; i < *size; ++i) {
                data[i] = -1.0f + temp[i] * kToFloat;
                assert(data[i] <= 1.0f || data[i] >= -1.0f);
            }
        } else {
            SetLastError(ErrorCode::UNABLE_TO_IMPORT_FILE);
            SetErrorDetails(
                "Unsupported audio file format, neither 32-bit IEEE float audio nor 16/8 bit PCM (format code = " 
                + std::to_string(formatChunk.GetAudioFormat()) + ")");

            ifs.close();
            return nullptr;
        }

    } else {
        SetLastError(ErrorCode::UNABLE_TO_IMPORT_FILE);
        SetErrorDetails(
            "Unsupported audio file format, neither 32-bit IEEE float audio nor 16/8 bit PCM (format code = " 
            + std::to_string(formatChunk.GetAudioFormat()) + ")");

        ifs.close();
        return nullptr;
    }

    // Close stream.
    ifs.close();

    return data.release();
}

float *ExtractChannel(float *data, int size, int numChannels, int channel, int *resultSize)
{
    if (size % numChannels != 0) {
        SetLastError(INVALID_PARAMETER);
        SetErrorDetails("Size of audio data mismatch number of specified channels");
        return nullptr;
    }

    int channelSize = size / numChannels;
    float* result = new float[channelSize];

    for (int i = 0; i < channelSize; ++i) {
        result[i] = data[channel + i * numChannels];
    }

    if (resultSize) {
        *resultSize = channelSize;
    }

    return result;
}

float *InterleaveChannels(float* ch1Data, float* ch2Data, int size, int *resultSize)
{
    float* result = new float[size * 2];

    for (int i = 0, j = 0; i < size; ++i) {
        result[j++] = ch1Data[i];
        result[j++] = ch2Data[i];
    }

    if (resultSize) {
        *resultSize = size * 2;
    }

    return result;
}

float *ConvertSampleRate(float *data, int size, int srcRate, int dstRate, int *resultSize)
{
    AudioBuffer src(srcRate, data, size);

    size_t n = static_cast<size_t>(dstRate * src.GetDuration() + static_cast<double>(0.5));
    float *result = new float[n];
    if (resultSize) {
        *resultSize = static_cast<int>(n);
    }

    double dt = src.GetDuration() / n;
    double t = 0;
    // std::cout << "duration = " << src.GetDuration() << std::endl;
    // std::cout << "duration = " << (dt * n) << std::endl;
    // double x = src.GetDuration();
    // double y = dt * n;
    for (size_t i = 0; i < n; ++i) {
        result[i] = src.GetSample(i * dt);
    }

    return result;
}