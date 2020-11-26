/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <cstddef>
#include <functional>

/**
 * @file
 * @brief This file defines public functions related to JACk2 using AST utilities API-Level 0.
 */

// example: void MyProcessFunc(float *in, float *out, unsigned int n) {}
using ProcessFunc = std::function<void (float* inSamples, float* outSamples, unsigned int numSamples)>;

int InitJackClient(const char name[]);
void QuitJackClient();
bool IsJackClientTerminated();
int SetJackProcessFunction(ProcessFunc func);
unsigned int GetSampleRate();
