/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#ifdef ASTU_EXPORT
#define ASTU_API __declspec(dllexport)
#elif defined(ASTU_IMPORT)
#define ASTU_API __declspec(dllimport)
#else
#define ASTU_API
#endif