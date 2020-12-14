/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
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
#include <algorithm>
#include <memory>

#include "VersionInfo.h"
#include "audio/WaveCodec.h"
#include "audio/AudioBuffer.h"
#include "Image.h"
#include "gfx/BmpCodec.h"
#include "gfx/Quadtree.h"
#include "gfx/PatternRenderer.h"
#include "math/Random.h"

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
    "The operation is not supported",
    "The current state is invalid for this operation",
    "Simple Direct Layer (SDL) error",
    "JACK Audio Connection Kit error",
    "Application error",    
    "Invalid error code",
    };

/////////////////////////////////////////////////
/////// Globals
/////////////////////////////////////////////////

std::chrono::time_point<std::chrono::steady_clock> startTime;
std::chrono::time_point<std::chrono::steady_clock> stopTime;

std::unique_ptr<astu::Image> lvl0Image = std::make_unique<Image>(512, 512);
std::shared_ptr<UnionPattern> rootPattern = std::make_shared<UnionPattern>();
std::shared_ptr<Quadtree> quadTree = std::make_shared<Quadtree>(5, 5);
std::unique_ptr<IPatternRenderer> patternRenderer = std::make_unique<AntiAlisaingPatternRenderer>();
Color lvl0DrawColor(1, 1, 1);
Color lvl0ClearColor(0, 0, 0);
std::ifstream ifs;
enum FileIoStatus {NoFile, InputFile, OutputFile};
FileIoStatus fioStatus = NoFile;
std::string tempString;

/** Stores input string read from standard input using AskString function. */
std::vector<std::unique_ptr<std::string>> inputStrings;

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

const char* GetLastErrorMessage()
{
    return GetErrorMessage(GetLastError());
}

void SetLastError(int errorCode)
{
    gLastError = errorCode;
    gErrorDetails.clear();
}

bool HasError()
{
    return GetLastError() != ErrorCode::NO_ERROR;
}

void SetErrorDetails(const std::string& detail)
{
    gErrorDetails = detail;
}

const char* GetErrorDetails()
{
    return gErrorDetails.c_str();
}

void SetErrorDetails(const char *text)
{
    SetErrorDetails(std::string(text));
}


/////////////////////////////////////////////////
/////// I/O Functions
/////////////////////////////////////////////////

void SayHello()
{
    cout << "Hello AST World :-)" << endl;
}

void SayError()
{
    std::cout << "An error has occurred: " << GetLastErrorMessage() << std::endl;
    std::cout << GetErrorDetails() << std::endl;
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
    cout << GetVersionInfo() << endl;
}

void SayElapsedTime(const char* text)
{
    if (text) {
        cout << text << " ";
    }
    
    auto dt = chrono::duration_cast<chrono::milliseconds>(stopTime - startTime).count();

    if (dt == 0) {
        dt = chrono::duration_cast<chrono::nanoseconds>(stopTime - startTime).count();
        double dtms = (dt / 1000000.0);
        cout << dtms << " ms" << endl;
        return;
    }

    int hr = static_cast<int>(dt / (1000 * 60 * 24));
    dt -= hr * (1000 * 60 * 24);

    if (hr > 0) {
        cout << hr << " hr ";
    }

    int min = static_cast<int>(dt / (1000 * 60));
    dt -= min * (1000 * 60);

    if (min > 0 || hr > 0) {
        cout << min << " min ";
    }

    int sec = static_cast<int>(dt / 1000);
    dt -= sec * 1000;

    if (sec > 0 || hr > 0 || min > 0) {
        cout << sec << " sec ";
    }
    if (dt > 0) {
        cout << dt << " ms";
    }

    cout << endl;
}

void SkipInputLine()
{
    std::getline(cin, tempString);    
}

int AskInt(const char* text)
{
    int result;
    if (text) {
        cout << text << " ";
    }
    cin >> result;

    // Skip the rest of the input line
    // and especially eat 'newline' to make AskString work again.
    SkipInputLine();

    return result;
}

double AskDouble(const char* text)
{
    double result;
    if (text) {
        cout << text << " ";
    }
    cin >> result;

    // Skip the rest of the input line
    // and especially eat 'newline' to make AskString work again.
    SkipInputLine();

    return result;
}

float AskFloat(const char* text)
{
    float result;
    if (text) {
        cout << text << " ";
    }
    cin >> result;

    // Skip the rest of the input line
    // and especially eat 'newline' to make AskString work again.
    SkipInputLine();

    return result;
}

const char* AskString(const char* text)
{
    if (text) {
        cout << text << " ";
    }

    auto result = std::make_unique<std::string>();    
    std::getline(cin, *result);
    inputStrings.push_back(std::move(result));

    return inputStrings.back()->c_str();
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

int Minimum(int a, int b)
{
    return std::min(a, b);
}

int Minimum(int a, int b, int c)
{
    return std::min(a, std::min(b, c));
}

int Maximum(int a, int b)
{
    return std::max(a, b);
}

int Maximum(int a, int b, int c)
{
    return std::max(a, std::max(b, c));
}

double GetRandomDouble(double minValue, double maxValue)
{
    if (minValue == 0 && maxValue == 1) {
        return Random::GetInstance().NextDouble();
    } else {
        return minValue + Random::GetInstance().NextDouble() * (maxValue - minValue);
    }
}

int GetRandomInt(int minValue, int maxValue)
{
    return minValue + static_cast<int>(Random::GetInstance().NextDouble() * (maxValue - minValue));
}

int RoundToInt(double value)
{
    return static_cast<int>(value + 0.5);
}

int GreatestCommonDivisor(int a, int b)
{
    if (b == 0) {
        return a;
    }

    return GreatestCommonDivisor(b, a % b);
}

int LowestCommonMultiple(int a, int b)
{
    return (a / GreatestCommonDivisor(a, b)) * b;
}

void Shuffle(int *values, int numValues)
{
    for (int i = numValues - 1; i > 0; --i) {
        int n = GetRandomInt(0, i + 1);
        int tmp = values[i];
        values[i] = values[n];
        values[n] = tmp;
    }           
}

bool IsBitSet(int value, int bit)
{
   return value & (1 << bit);
}

int SetBit(int value, int bit)
{
    return value | (1 << bit);
}

int ClearBit(int value, int bit)
{
    return value & ~(1 << bit);
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
        SetLastError(ErrorCode::UNABLE_TO_OPEN_FILE_FOR_WRITING);
        SetErrorDetails(std::string("The file '") 
            + filename + "' could not be opened for writing");
        return GetLastError();
    }

    if (channels < 0 || channels > std::numeric_limits<uint16_t>::max()) {
        SetLastError(ErrorCode::INVALID_PARAMETER);
        SetErrorDetails("The specified number of channels is invalid."
            + std::string(" The channel parameter was set to ")
            + std::to_string(channels) + ", the valid range is between 0 and " 
            + std::to_string(std::numeric_limits<uint16_t>::max()) + ".");
        return GetLastError();        
    }

    if (sampleRate < 0) {
        SetLastError(ErrorCode::INVALID_PARAMETER);
        SetErrorDetails("The specified sample rate is invalid.");
        return GetLastError();        
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
        *size = 0; *sampleRate = 0; *channels = 0;
        return nullptr;
    }

    // Open input stream.
    std::ifstream ifs(filename, std::ios::in | std::ios::binary);

    if (!ifs.good()) {
        SetLastError(ErrorCode::UNABLE_TO_OPEN_FILE_FOR_READING);
        *size = 0; *sampleRate = 0; *channels = 0;
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
            *size = 0; *sampleRate = 0; *channels = 0;
            ifs.close();
            return nullptr;
        }
        formatChunk.ReadWithoutFourCC(ifs);

        // Read data chunk (without actual audio data).
        if (!FindChunk(ifs, dataChunk.GetFourCC())) {
            SetLastError(ErrorCode::UNABLE_TO_IMPORT_FILE);
            SetErrorDetails("Data chunk not found");
            *size = 0; *sampleRate = 0; *channels = 0;
            ifs.close();
            return nullptr;
        }
        dataChunk.ReadWithoutFourCC(ifs);

    } catch (std::runtime_error & e) {
        SetLastError(ErrorCode::UNABLE_TO_IMPORT_FILE);
        SetErrorDetails(e.what());
        ifs.close();

        *size = 0; *sampleRate = 0; *channels = 0;
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
            for (int i = 0; i < *size; ++i) {
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

            for (int i = 0; i < *size; ++i) {
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
            for (int i = 0; i < *size; ++i) {
                data[i] = -1.0f + temp[i] * kToFloat;
                assert(data[i] <= 1.0f || data[i] >= -1.0f);
            }
        } else {
            SetLastError(ErrorCode::UNABLE_TO_IMPORT_FILE);
            SetErrorDetails(
                "Unsupported audio file format, neither 32-bit IEEE float audio nor 16/8 bit PCM (format code = " 
                + std::to_string(formatChunk.GetAudioFormat()) + ")");

            ifs.close();
            *size = 0; *sampleRate = 0; *channels = 0;
            return nullptr;
        }

    } else {
        SetLastError(ErrorCode::UNABLE_TO_IMPORT_FILE);
        SetErrorDetails(
            "Unsupported audio file format, neither 32-bit IEEE float audio nor 16/8 bit PCM (format code = " 
            + std::to_string(formatChunk.GetAudioFormat()) + ")");

        ifs.close();
        *size = 0; *sampleRate = 0; *channels = 0;
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
        result[i] = static_cast<float>(src.GetSample(i * dt));
    }

    return result;
}

/////////////////////////////////////////////////
/////// File Functions
/////////////////////////////////////////////////



int OpenFile(const char *filename, bool openForReading)
{
    if (fioStatus != FileIoStatus::NoFile) {
        SetLastError(ErrorCode::NOT_SUPPORTED);
        SetErrorDetails("Another file has already been opened.");
        return GetLastError();
    }

    if (openForReading) {
        ifs.open(filename, std::ios::in);
        if (!ifs.good()) {
            SetLastError(ErrorCode::UNABLE_TO_OPEN_FILE_FOR_READING);
            SetErrorDetails(std::string("The file '") 
                + filename + "' could not be opened for reading.");
            return GetLastError();
        }
        fioStatus = FileIoStatus::InputFile;
    } else {
        SetLastError(ErrorCode::NOT_SUPPORTED);
        SetErrorDetails("Opening files for writing is not supported yet.");
        return GetLastError();
    }

    return ErrorCode::NO_ERROR;
}


int CloseFile()
{
    switch (fioStatus) {
    case FileIoStatus::InputFile:
        ifs.close();
        return ErrorCode::NO_ERROR;

    case FileIoStatus::OutputFile:
        SetLastError(ErrorCode::NOT_SUPPORTED);
        return GetLastError();

    case FileIoStatus::NoFile:
        SetLastError(ErrorCode::NOT_SUPPORTED);
        SetErrorDetails("No file is currently open.");
        return GetLastError();

    default:
        SetLastError(ErrorCode::INVALID_STATE);
        return GetLastError();
    }
}

bool Readable()
{
    return fioStatus == FileIoStatus::InputFile && ifs.good();
}

int ReadInt()
{
    if (fioStatus != FileIoStatus::InputFile) {
        SetLastError(ErrorCode::NOT_SUPPORTED);
        SetErrorDetails("No file has been opened for reading.");
        return 0;
    }

    int result;
    ifs >> result;

    return result;
}

int SkipLine()
{
    if (fioStatus != FileIoStatus::InputFile) {
        SetLastError(ErrorCode::NOT_SUPPORTED);
        SetErrorDetails("No file has been opened for reading.");
        return GetLastError();
    }
    
    std::string line;
    getline(ifs, line);
    return ErrorCode::NO_ERROR;
}


char ReadChar()
{
    if (fioStatus != FileIoStatus::InputFile) {
        SetLastError(ErrorCode::NOT_SUPPORTED);
        SetErrorDetails("No file has been opened for reading.");
        return 0;
    }

    char result = ifs.get();
    return result;
}

double ReadDouble()
{
    if (fioStatus != FileIoStatus::InputFile) {
        SetLastError(ErrorCode::NOT_SUPPORTED);
        SetErrorDetails("No file has been opened for reading.");
        return 0;
    }

    double result;
    ifs >> result;

    return result;
}

const char* ReadString()
{
    tempString.clear();
    if (fioStatus != FileIoStatus::InputFile) {
        SetLastError(ErrorCode::NOT_SUPPORTED);
        SetErrorDetails("No file has been opened for reading.");
        tempString.clear();
        return nullptr;
    }

    ifs >> tempString;

    return tempString.c_str();
}

bool CompareString(const char* s1, const char* s2)
{
    return std::string(s1) == std::string(s2); 
}


/////////////////////////////////////////////////
/////// Graphics Functions
/////////////////////////////////////////////////

int CreateImage(int w, int h)
{
    if (w <= 0 || h <= 0) {
        SetLastError(ErrorCode::INVALID_PARAMETER);
        SetErrorDetails("Vertical and horizontal resolution of an image must be greater zero.");
        return GetLastError();
    }

    if (lvl0Image->GetWidth() != w || lvl0Image->GetHeight() != h) {
        lvl0Image = std::make_unique<Image>(w, h);
    }

    ClearImage();

    return ErrorCode::NO_ERROR;
}

void ClearImage()
{
    rootPattern->Clear();
    rootPattern->Add(std::make_shared<UnicolorPattern>(lvl0ClearColor));
    rootPattern->Add(quadTree);
}

void SetDrawColor(int r, int g, int b, int a)
{
    lvl0DrawColor.Set(r, g, b, a);
}

void SetClearColor(int r, int g, int b)
{
    lvl0ClearColor.Set(r, g, b);
}

void DrawLine(double x0, double y0, double x1, double y1, double w)
{
    Vector2<double> v(x1 - x0, y1 - y0);

    double lng = v.Length();
    if (lng <= 0 || w <= 0) {
        return;
    }

    double a = v.Angle(Vector2<double>(1, 0));

    auto rect = std::make_shared<RectanglePattern>(lng, w);
    rect->Translate((x1 + x0) / 2, (y1 + y0) / 2);
    rect->Rotate(-a);
    rect->SetPattern(std::make_shared<UnicolorPattern>(lvl0DrawColor));
    quadTree->Add(rect);
}

void DrawCircle(double x, double y, double r) 
{
    if (r <= 0) {
        return;
    }
    auto circle = std::make_shared<CirclePattern>(r);
    circle->Translate(x, y);
    circle->SetPattern(std::make_shared<UnicolorPattern>(lvl0DrawColor));
    quadTree->Add(circle);
}

int WriteImage(const char* filename)
{
    quadTree->BuildTree();
    patternRenderer->Render(*rootPattern, *lvl0Image);

    BmpEncoder bmpEnc;
    bmpEnc.Encode(*lvl0Image, filename);

    return ErrorCode::NO_ERROR;
}

