/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#ifdef ASTU_EXPORT
#define ASTU_API __declspec(dllexport)
#elif ASTU_IMPORT
#define ASTU_API __declspec(dllimport)
#else
#define ASTU_API
#endif