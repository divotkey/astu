/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#define _USE_MATH_DEFINES
 
#include <cmath>
#include <iostream>
#include <chrono>

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

/////////////////////////////////////////////////
/////// Globals
/////////////////////////////////////////////////
std::chrono::time_point<std::chrono::steady_clock> startTime;
std::chrono::time_point<std::chrono::steady_clock> stopTime;

/////////////////////////////////////////////////
/////// I/O Functions
/////////////////////////////////////////////////

void SayHello()
{
    cout << "Hello AST World :-)" << endl;
}

void Say(const char* text)
{
    if (text) {
        std::cout << text;
    }
    std::cout << std::endl;
}

void Say(int value)
{
    std::cout << value << std::endl;
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

